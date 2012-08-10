///*
// * LPA2.cpp
// *
// *  Created on: 22 juil. 2011
// *      Author: manuel
// */

#include "GraphPartition.hpp"
//#include "Modularity.hpp"
#include "ILinks.hpp"
//#include "Utils.hpp"

GraphPartition::GraphPartition(ILinks const & _graph) :
		IGraphPartition(_graph), _partition(_graph.nbLinks()), _labelDegree(
				_graph.nbLinks(), 0) {
	//	startWith(IntVector(nbLinks(), 0));
}

GraphPartition::~GraphPartition() {

}

//// attention, plus besoin d'avoir des entiers contigus
//void GraphPartition::set(IPartition const & location) {
//	_size.assign(nbLinks(), 0);
//	_usedLabel.clear();
//	_unUsedLabel.fill();
//	_labelDegree.assign(nbLinks(), 0);
//	_labelLists.assign(nbLinks(), IntList());
//	for (size_t n(0); n < nbLinks(); ++n) {
//		size_t const l(location.label(n));
//		label(n) = l;
//		_usedLabel.insert(l);
//		_unUsedLabel.erase(l);
//		_labelLists[l].push_front(n);
//		_nodePosition[n] = _labelLists[l].begin();
//		_labelDegree[l] += graph().degree(n);
//		++_size[l];
//	}
//}

//void GraphPartition::computeIntraMesure() {
//	_labelIntra.assign(nbObs(), 0);
//	for (size_t n(0); n < nbObs(); ++n) {
//		for (Graph::Row::const_iterator row(_graph[n].begin()); row
//				!= _graph[n].end(); ++row) {
//			if (labelOfNode(n) == labelOfNode(row->first))
//				_labelIntra[labelOfNode(n)] += row->second;
//		}
//	}
//}
//void GraphPartition::buildLabelGraphPartition(size_t l) {
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

double GraphPartition::intra(size_t label) {
	double result(0);
	FOR_EACH_CONST(node1, list(label)){
	result+=intra(node1,label);
}
	return result;
}
double GraphPartition::intra(size_t node, size_t l) {
	double result(0);
	LinksIterator ite(graph().links(node));
	while (ite.goNext()) {
		if (l == label(ite.index())) {
			result += ite.value();
		}
	}
	return result;
}

void GraphPartition::intra(size_t node, DoubleVector & values,
		bool const & reset) {
	values.resize(nbLinks());
	if (reset)
		std::fill_n(values.begin(), nbLinks(), 0.0);
	LinksIterator ite(graph().links(node));
	while (ite.goNext()) {
		values[label(ite.index())] += ite.value();
	}
}

double GraphPartition::computeDegreeOfLabel(size_t l) const {
	double res(0);
	FOR_EACH_CONST(n ,list(l)) {
	res += graph().weight(n);
}
	return res;
}

bool GraphPartition::checkPosition() const {
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
bool GraphPartition::check() const {
	checkDegrees();
	return true;
}

bool GraphPartition::checkDegrees() const {
	for (size_t l(0); l < nbLinks(); ++l)
		if (!checkDegree(l))
			return false;
	return true;
}

bool GraphPartition::checkDegree(size_t const l) const {
	double verif(0);
	//		TRACE("LABEL "<<l<<std::endl);
	FOR_EACH_CONST( n , list(l)) {
	//			TRACE_N(*n);
	verif += graph().weight(n);
}
	if (fabs(verif - degreeOfLabel(l)) > 1e-10) {
		std::cout << "WRONG DEGREE OF LABEL " << l << " = " << degreeOfLabel(l)
				<< " sould be " << verif << std::endl;
		return false;
	}

	return true;
}
//IntVector GraphPartition::sortLocation() const {
//	return SortLocation(nodeLabel());
//}

void GraphPartition::shift(size_t n, size_t l) {
	if (label(n) != l) {
		degreeOfLabel(l) += graph().weight(n);
		degreeOfLabel(label(n)) -= graph().weight(n);
		_partition.shift(n, l);
	}
}
// fusion de deux labels
size_t GraphPartition::fusion(size_t l1, size_t l2) {
	IntSet buffer;
	Insert(list(l1), buffer);
	FOR_EACH_CONST(n , buffer) shift(n, l2);
	return l2;

}

size_t GraphPartition::nbObs() const {
	assert(_partition.nbObs() == graph().nbLinks());
	return _partition.nbObs();
}
size_t GraphPartition::nbLinks() const {
	return graph().nbLinks();
}
size_t GraphPartition::nbLabels() const {
	return _partition.nbLabels();
}

size_t GraphPartition::sizeOfLabel(size_t l) const {
	return _partition.sizeOfLabel(l);
}

size_t & GraphPartition::sizeOfLabel(size_t l) {
	return _partition.sizeOfLabel(l);
}
double const & GraphPartition::degreeOfLabel(size_t l) const {
	return _labelDegree[l];
}

double & GraphPartition::degreeOfLabel(size_t l) {
	return _labelDegree[l];
}

IndexedList const & GraphPartition::usedLabel() const {
	return _partition.usedLabel();
}
IndexedList const & GraphPartition::unUsedLabel() const {
	return _partition.unUsedLabel();
}

IntList & GraphPartition::list(size_t label) {
	return _partition.list(label);
}
IntList const & GraphPartition::list(size_t label) const {
	return _partition.list(label);
}

size_t GraphPartition::getUnUsedLabel() const {
	return _partition.getUnUsedLabel();
}

bool GraphPartition::allLabelsUsed() const {
	return _partition.allLabelsUsed();
}

bool GraphPartition::isUsed(size_t l) const {
	return _partition.isUsed(l);
}

IndexedList & GraphPartition::unUsed() {
	return _partition.unUsed();
}

IndexedList const & GraphPartition::unUsed() const {
	return _partition.unUsed();
}

IndexedList & GraphPartition::used() {
	return _partition.used();
}

IndexedList const & GraphPartition::used() const {
	return _partition.used();
}
