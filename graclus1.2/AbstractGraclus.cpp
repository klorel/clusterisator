/*
 * IGraclus.cpp
 *
 *  Created on: 2 août 2012
 *      Author: manuel
 */

#include "AbstractGraclus.hpp"
#include "common.h"

int boundary_points;
int spectral_initialization;
int cutType; //cut type, default is normalized cut
int memory_saving; // forbid using local search or empty cluster removing
//char mlwkkm_fname[256]; //used to store coarsest file

AbstractGraclus::AbstractGraclus() {
	init();
}

AbstractGraclus::~AbstractGraclus() {
	free();
}

unsigned int AbstractGraclus::get(unsigned const int & vtx) const {
	return _partition[vtx];
}

double AbstractGraclus::score() const {
	return _score;
}

void AbstractGraclus::init() {
	InitGraph(&_graph);
}
void AbstractGraclus::free() {
	delete _graph.xadj;
	delete _graph.adjncy;
	delete _graph.vwgt;
	delete _graph.adjwgt;
}

void AbstractGraclus::setRow(unsigned const int & i, Row const & row) {
	FOR_EACH_CONST(e , row) {
		_graph.adjncy[_k] = e.first;
		if (_isWeighted)
			_graph.adjwgt[_k] = static_cast<int>(floor(e.second));
		else
			_graph.adjwgt[_k] = 1;
		_k++;
	}
	_graph.xadj[i + 1] = _k;
}

void AbstractGraclus::allocate(size_t const & nbNodes, size_t const & nbEdges,
		bool const & isWeighted) {
	_isWeighted = isWeighted;
	_graph.nvtxs = nbNodes;
	_graph.nedges = nbEdges;

	_graph.nedges *= 2;

	_graph.xadj = idxsmalloc(_graph.nvtxs + 1, 0, 0);
	_graph.adjncy = idxmalloc(_graph.nedges, 0);

	_graph.ncon = 1;
	_graph.vwgt = NULL;
	_graph.adjwgt = idxmalloc(_graph.nedges, 0);
	/* Start reading the graph file */
	_graph.xadj[0] = 0;
	_k = 0;
	_wgtflag = (_isWeighted ? 1 : 0);
}
//void AbstractGraclus::readGraph(Graph const & input) {
//	allocate(input.nbNodes(), input.nbEdge(), true);
//	for (int i = 0; i < _graph.nvtxs; i++) {
//		setRow(i, input.row(i));
//
//	}
//}

void AbstractGraclus::check() {
	if (_k != _graph.nedges) {
		printf(
				"------------------------------------------------------------------------------\n");
		printf("***  I detected an error in your input file  ***\n\n");
		printf(
				"In the first line of the file, you specified that the graph contained\n%d edges. However, I only found %d edges in the file.\n",
				_graph.nedges / 2, _k / 2);
		if (2 * _k == _graph.nedges) {
			printf(
					"\n *> I detected that you specified twice the number of edges that you have in\n");
			printf(
					"    the file. Remember that the number of edges specified in the first line\n");
			printf(
					"    counts each edge between vertices v and u only once.\n\n");
		}
		printf(
				"Please specify the correct number of edges in the first line of the file.\n");
		printf(
				"------------------------------------------------------------------------------\n");
		exit(0);
	}
}
void AbstractGraclus::launch(int nparts) {
	_partition.resize(_graph.nvtxs);
	if (_graph.nvtxs <= 0) {
		printf("Empty graph. Nothing to do.\n");
		exit(0);
	}

	float lbvec[MAXNCON];
	//	stoptimer(IOTmr);

	int levels = amax((_graph.nvtxs)/(40*log2_metis(nparts)), 20*(nparts));
	//levels = graph.nvtxs/128;
	//	printf(
	//			"\n----------------------------------------------------------------------\n");
	//	printf("%s", MLKKMTITLE);
	//	printf("Graph Information:\n");
	//	printf("  Name: %s, \n  #Vertices: %d, #Edges: %d, ", "noname",
	//			_graph.nvtxs, _graph.nedges / 2);
	//	if (_graph.ncon > 1)
	//		printf("  Balancing Constraints: %d\n", _graph.ncon);

	idxtype * part = idxmalloc(_graph.nvtxs, 0);
	int options[11];
	options[0] = 0;
	//	starttimer(METISTmr);
	int numflag = 0;
	int clusteringEva = 0;
	int edgecut = 0;

	int cutType = 0;
	int chain_length = 0;

	if (clusteringEva > 0) {
		//nparts = readClustering(clusterIDFile, part, graph.nvtxs);
		//		printf("#Clusters: %d\n\nClustering file:\n  %s\n", nparts,
		//				clusterIDFile);
	} else {
		//		printf("#Clusters: %d\n", nparts);
		if (_graph.ncon == 1) {
			// modification
			/*METIS_PartGraphKway(&graph.nvtxs, graph.xadj, graph.adjncy, graph.vwgt, graph.adjwgt,
			 &wgtflag, &numflag, &nparts, options, &edgecut, part);
			 */
			MLKKM_PartGraphKway(&_graph.nvtxs, _graph.xadj, _graph.adjncy,
					_graph.vwgt, _graph.adjwgt, &_wgtflag, &numflag, &nparts,
					&chain_length, options, &edgecut, part, levels);

			/* ends */
		} else {
			//			for (i = 0; i < graph.ncon; i++)
			//				rubvec[i] = HORIZONTAL_IMBALANCE;
			/*
			 METIS_mCPartGraphKway(&graph.nvtxs, &graph.ncon, graph.xadj, graph.adjncy, graph.vwgt,
			 graph.adjwgt, &wgtflag, &numflag, &nparts, rubvec, options, &edgecut, part);
			 */
		}
	}
	//	stoptimer(METISTmr);
	ComputePartitionBalance(&_graph, nparts, part, lbvec);

	if (cutType == NCUT) {
		_score = ComputeNCut(&_graph, part, nparts);
		//		printf("\nNormalized-Cut... \n   Cut value: %7f, Balance: ", _score);
	} else {
		_score = ComputeRAsso(&_graph, part, nparts);
		//		printf("\nRatio Association...  \n  Association value: %7f, Balance: ",
		//				_score);
	}

	//printf("  %d-way Edge-Cut: %7d\n", nparts, ComputeCut(&graph, part));
	//	for (int i = 0; i < _graph.ncon; i++)
	//		printf("%5.2f ", lbvec[i]);
	//	printf("\n");

	if (clusteringEva == 0) {
		//		starttimer(IOTmr);
		//		WritePartition(filename, part, graph.nvtxs, nparts);

		for (int i(0); i < _graph.nvtxs; ++i)
			_partition[i] = part[i];

		//		stoptimer(IOTmr);
	}
	//	stoptimer(TOTALTmr);

	//	printf("\nTiming Information:\n");
	//	printf("  I/O:          \t\t %7.3f\n", gettimer(IOTmr));
	//	printf("  Clustering:   \t\t %7.3f   (Graclus time)\n", gettimer(METISTmr));
	//	printf("  Total:        \t\t %7.3f\n", gettimer(TOTALTmr));
	//	printf(
	//			"----------------------------------------------------------------------\n");
}
