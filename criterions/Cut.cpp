/*
 * Cut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Cut.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"

void Cut::getCut(IGraphPartition const & data, DoubleVector & cut) const {
	cut.assign(data.nbObs(), 0);
//	for (size_t n(0); n < data.nbNodes(); ++n) {
//		size_t const & l(data.label(n));
//		FOR_EACH_CONST(e , data.graph().links(n)) {
//		if (data.label(e.first) != l)
//		cut[l] += e.second;
//	}
//}
}

Double Cut::getCut(IGraphPartition const & data, size_t const & l) const {
	Double cut(0);
//	FOR_EACH_CONST (n , data.list(l)) {
//	size_t const & l(data.label(n));
//	FOR_EACH_CONST( e , data.graph().links(n)) {
//		if (data.label(e.first) != l)
//		cut += e.second;
//	}
//}
	return cut;
}

Double2 Cut::getDeltaShift(IGraphPartition const & data, size_t const & node,
		size_t const & newLabel) const {
	size_t const oldLabel(data.label(node));
	Double2 delta(0, 0);
	if (oldLabel != newLabel) {

	}
	return delta;

}
Double2 Cut::getDeltaShift(IGraphPartition const & data, size_t const & node,
		size_t const & newLabel, DoubleVector const & intra) const {

	size_t const oldLabel(data.label(node));
	Double2 delta(0, 0);
	if (oldLabel != newLabel) {
		delta.first = 2 * intra[oldLabel] - data.graph().weight(node);
		delta.second = -2 * intra[oldLabel] + data.graph().weight(node);
	}
	return delta;

}
