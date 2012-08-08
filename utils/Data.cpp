/*
 * LPA2.cpp
 *
 *  Created on: 22 juil. 2011
 *      Author: manuel
 */

#include "Data.hpp"
//#include "Modularity.hpp"
#include "IGraph.hpp"
//#include "Utils.hpp"

Data::Data(IGraph const & _graph) :
IExtendedPartition(_graph), _labelDegree(graph().nbNodes(), 0), _labelLists(
		graph().nbNodes()), _nodeLabel(graph().nbNodes(),
				graph().nbNodes()), _nodePosition(graph().nbNodes()), _size(
						graph().nbNodes(), 0), _usedLabel(graph().nbNodes()), _unUsedLabel(
								graph().nbNodes()) {
	//	startWith(IntVector(nbNodes(), 0));
}

Data::~Data() {

}

// attention, plus besoin d'avoir des entiers contigus
void Data::startWith(IPartition const & location) {
	_size.assign(nbNodes(), 0);
	_usedLabel.clear();
	_unUsedLabel.fill();
	_labelDegree.assign(nbNodes(), 0);
	_nodeLabel = location;
	_labelLists.assign(nbNodes(), IntList());
	for (size_t n(0); n < nbNodes(); ++n) {
		size_t const l(location.label(n));
		_usedLabel.insert(l);
		_unUsedLabel.erase(l);
		_labelLists[l].push_front(n);
		_nodePosition[n] = _labelLists[l].begin();
		_labelDegree[l] += graph().degree(n);
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

double Data::intra(size_t const & label) {
	double result(0);
	FOR_EACH_CONST(node1, list(label)){
		result+=intra(node1,label);
	}
	return result;
}
double Data::intra(size_t const & node, size_t const & label) {
	double result(0);
	FOR_EACH_CONST(node2, graph().adjacentList(node)) {
		if (label == _nodeLabel.label(node2.first)) {
			result+=node2.second;
		}
	}
	return result;
}
double Data::computeDegreeOfLabel(size_t const &l) const {
	double res(0);
	FOR_EACH_CONST(n ,list(l)) {
		res += graph().degree(n);
	}
	return res;
}

bool Data::checkPosition() const {
	FOR_EACH_CONST(l ,usedLabel()) {
		//		TRACE_N(*l);
		DisplayContainer(std::cout << "\nlabel " << l << " : ", list(l));
	}
	std::cout << std::endl;
	FOR_EACH_CONST(l , usedLabel()) {
		assert(sizeOfLabel(l)>0);
		FOR_EACH_CONST(n , list(l)) {
			if (label(n) != l) {
				std::cout << n;
				std::cout << " not in " << l;
				std::cout << " but in " << label(n);
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
	FOR_EACH_CONST(l , usedLabel()) {
		//		TRACE_N(*l);
		if (sizeOfLabel(l) == 0)
			std::cout << "error on size of label " << l << "\n";
		FOR_EACH_CONST( n , list(l)) {
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
	FOR_EACH_CONST( n , list(l)) {
		//			TRACE_N(*n);
		verif += graph().degree(n);
	}
	if (fabs(verif - degreeOfLabel(l)) > 1e-10) {
		std::cout << "WRONG DEGREE OF LABEL " << l << " = " << degreeOfLabel(l)
						<< " sould be " << verif << std::endl;
		return false;
	}

	return true;
}
//IntVector Data::sortLocation() const {
//	return SortLocation(nodeLabel());
//}

void Data::shift(size_t const& n, size_t const & l) {
	if (label(n) != l) {
		degreeOfLabel(l) += graph().degree(n);
		degreeOfLabel(label(n)) -= graph().degree(n);
		--sizeOfLabel(label(n));
		_labelLists[label(n)].erase(_nodePosition[n]);
		if (sizeOfLabel(label(n)) == 0) {
			_usedLabel.erase(label(n));
			_unUsedLabel.insert(label(n));
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
		_nodeLabel.label(n) = l;
	}
}
// fusion de deux labels
size_t Data::fusion(size_t const & l1, size_t const & l2) {
	IntSet buffer;
	Insert(list(l1), buffer);
	FOR_EACH_CONST(n , buffer) shift(n, l2);
	return l2;

}
IPartition const & Data::nodeLabel() const {
	return _nodeLabel;
}

size_t Data::nbNodes() const {
	return graph().nbNodes();
}
size_t Data::nbEdges() const {
	return graph().nbEdges();
}
size_t Data::nbLabels() const {
	return _usedLabel.size();
}

size_t const & Data::sizeOfLabel(size_t const & l) const {
	return _size[l];
}

size_t & Data::sizeOfLabel(size_t const & l) {
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

size_t Data::label(size_t const &n) const {
	return _nodeLabel.label(n);
}

size_t & Data::label(size_t const &n) {
	return _nodeLabel.label(n);
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
