/*
 * LPA2.cpp
 *
 *  Created on: 22 juil. 2011
 *      Author: manuel
 */

#include "Data.hpp"
//#include "Modularity.hpp"
#include "Graph.hpp"
//#include "Utils.hpp"

Data::Data(Graph const & graph) :
		_graph(graph), _labelDegree(_graph.nbNodes(), 0), _labelLists(
				_graph.nbNodes()), _nodeLabel(_graph.nbNodes(),
				_graph.nbNodes()), _nodePosition(_graph.nbNodes()), _size(
				_graph.nbNodes(), 0), _usedLabel(_graph.nbNodes()), _unUsedLabel(
				_graph.nbNodes()) {

}

Data::~Data() {

}

Data::Data(Data const & rhs) :
		_graph(rhs._graph), _labelDegree(rhs._labelDegree), _labelLists(
				rhs.nbNodes()), _nodeLabel(rhs._nodeLabel), _nodePosition(
				rhs.nbNodes(), list(0).end()), _size(rhs._size), _usedLabel(
				rhs._usedLabel), _unUsedLabel(rhs._unUsedLabel) {
	assert(nbNodes() == rhs.nbNodes());
	for (size_t n(0); n < nbNodes(); ++n) {
		_labelLists[rhs.labelOfNode(n)].push_front(n);
		_nodePosition[n] = _labelLists[rhs.labelOfNode(n)].begin();
	}
}
// attention, plus besoin d'avoir des entiers contigus
void Data::startWith(IntVector const & location) {
	_size.assign(nbNodes(), 0);
	_usedLabel.clear();
	_unUsedLabel.fill();
	_labelDegree.assign(nbNodes(), 0);
	_nodeLabel = location;
	_labelLists.assign(nbNodes(), IntList());
	for (size_t n(0); n < nbNodes(); ++n) {
		size_t const & l(location[n]);
		_usedLabel.insert(l);
		_unUsedLabel.erase(l);
		_labelLists[l].push_front(n);
		_nodePosition[n] = _labelLists[l].begin();
		_labelDegree[l] += _graph.degree(n);
		++_size[l];
	}
}
//void Data::computeIntraMesure() {
//	_labelIntra.assign(nbObs(), 0);
//	for (size_t n(0); n < nbObs(); ++n) {
//		for (Graph::Row::const_iterator row(_graph[n].begin()); row
//				!= _graph[n].end(); ++row) {
//			if (labelOfNode(n) == labelOfNode(row->first))
//				_labelIntra[labelOfNode(n)] += row->second;
//		}
//	}
//}
//void Data::buildLabelData(size_t l) {
//
//	_labelGraph[l].clear();
//	for (IntList::const_iterator nIte(begin(l)); nIte != end(l); ++nIte) {
//		size_t n(*nIte);
//		for (Graph::Row::const_iterator row(_graph[n].begin()); row
//				!= _graph[n].end(); ++row) {
//			if (l != labelOfNode(row->first)) {
//
//				Int2Double::iterator found(
//						_labelGraph[l].find(labelOfNode(row->first)));
//				if (found == _labelGraph[l].end())
//					_labelGraph[l].insert(
//							Int2Double::value_type(labelOfNode(row->first),
//									row->second));
//				else
//					found->second += row->second;
//			}
//		}
//	}
//}

double Data::computeDegreeOfLabel(size_t const &l) const {
	double res(0);
	for (auto const & n : list(l)) {
		res += _graph.degree(n);
	}
	return res;
}

bool Data::checkPosition() const {
	for (auto const & l : usedLabel()) {
		//		TRACE_N(*l);
		DisplayContainer(std::cout << "\nlabel " << l << " : ", list(l));
	}
	std::cout << std::endl;
	for (auto const & l : usedLabel()) {
		assert(sizeOfLabel(l)>0);
		for (auto const & n : list(l)) {
			if (labelOfNode(n) != l) {
				std::cout << n;
				std::cout << " not in " << l;
				std::cout << " but in " << labelOfNode(n);
				std::cout << std::endl;
				assert(false);
			}
		}
	}
	return true;
}
bool Data::check() const {
	DoubleVector v(nbNodes(), 0);
	//	for (size_t n(0); n < nbNodes(); ++n) {
	//		std::cout << std::setw(4) << n;
	//		std::cout << std::setw(4) << labelOfNode(n);
	//		std::cout << std::setw(4) << degreeOfNode(n);
	//		std::cout << std::setw(4) << degreeOfLabel(labelOfNode(n));
	//		std::cout << std::endl;
	//	}
	checkDegrees();
	for (auto const & l : usedLabel()) {
		//		TRACE_N(*l);
		if (sizeOfLabel(l) == 0)
			std::cout << "error on size of label " << l << "\n";
		for (auto const & n : list(l)) {
			if (n != *_nodePosition[n]) {
				std::cout << n << " iterator in label " << l << " is wrong\n";
			}
		}
	}
	return true;
}

bool Data::checkDegrees() const {
	for (size_t l(0); l < nbNodes(); ++l)
		if (!checkDegree(l))
			return false;
	return true;
}

bool Data::checkDegree(size_t const l) const {
	double verif(0);
	//		TRACE("LABEL "<<l<<std::endl);
	for (auto const & n : list(l)) {
		//			TRACE_N(*n);
		verif += _graph.degree(n);
	}
	if (fabs(verif - degreeOfLabel(l)) > 1e-10) {
		std::cout << "WRONG DEGREE OF LABEL " << l << " = " << degreeOfLabel(l)
				<< " sould be " << verif << std::endl;
		return false;
	}

	return true;
}
IntVector Data::sortLocation() const {
	return SortLocation(nodeLabel());
}

void Data::shift(size_t const& n, size_t const & l) {
	if (labelOfNode(n) != l) {
		degreeOfLabel(l) += _graph.degree(n);
		degreeOfLabel(labelOfNode(n)) -= _graph.degree(n);
		--sizeOfLabel(labelOfNode(n));
		_labelLists[labelOfNode(n)].erase(_nodePosition[n]);
		if (sizeOfLabel(labelOfNode(n)) == 0) {
			_usedLabel.erase(labelOfNode(n));
			_unUsedLabel.insert(labelOfNode(n));
			//			std::cout << "Result : " << std::endl;
			//			for (IndexedList::const_iterator lIte(begin()); lIte != end(); ++lIte) {
			//				std::cout << std::setw(4) << *lIte;
			//				std::cout << std::setw(4) << sizeOfLabel(*lIte);
			//				std::cout << std::setw(4) << isUsed(*lIte);
			//				std::cout << std::endl;
			//			}
			//			std::cout << "-----------------" << std::endl;
		}
		if (sizeOfLabel(l) == 0) {
			_unUsedLabel.erase(l);
			_usedLabel.insert(l);
		}
		++sizeOfLabel(l);
		_labelLists[l].push_front(n);
		_nodePosition[n] = _labelLists[l].begin();
		_nodeLabel[n] = l;
	}
}
// fusion de deux labels
void Data::fusion(size_t const & l1, size_t const & l2) {
	IntSet buffer;
	Insert(list(l1), buffer);
	for (auto const & n : buffer)
		shift(n, l2);

}
IntVector const & Data::nodeLabel() const {
	return _nodeLabel;
}

Graph const & Data::graph() const {
	return _graph;
}
size_t Data::nbNodes() const {
	return _graph.nbNodes();
}
size_t Data::nbEdges() const {
	return _graph.nbEdge();
}
size_t Data::nbLabels() const {
	return _usedLabel.size();
}

size_t const & Data::sizeOfLabel(size_t const & l) const {
	return _size[l];
}

size_t & Data::sizeOfLabel(size_t const & l) {
	if (l >= nbNodes()) {
		TRACE_N(l);
	}
	return _size[l];
}
double const & Data::degreeOfLabel(size_t const & l) const {
	return _labelDegree[l];
}

double & Data::degreeOfLabel(size_t const & l) {
	return _labelDegree[l];
}

IndexedList const & Data::usedLabel() const {
	return _usedLabel;
}
IndexedList const & Data::unUsedLabel() const {
	return _unUsedLabel;
}

IntList & Data::list(size_t const & label) {
	return _labelLists[label];
}
IntList const & Data::list(size_t const & label) const {
	return _labelLists[label];
}

size_t const & Data::labelOfNode(size_t n) const {
	return _nodeLabel[n];
}

size_t & Data::labelOfNode(size_t n) {
	return _nodeLabel[n];
}
size_t Data::getUnUsedLabel() const {
	return _unUsedLabel.front();
}

bool Data::allLabelsUsed() const {
	return _unUsedLabel.empty();
}

bool Data::isUsed(size_t const & l) const {
	return _usedLabel.contains(l);
}

IndexedList & Data::unUsed() {
	return _unUsedLabel;
}

IndexedList const & Data::unUsed() const {
	return _unUsedLabel;
}

IndexedList & Data::used() {
	return _usedLabel;
}

IndexedList const & Data::used() const {
	return _usedLabel;
}
