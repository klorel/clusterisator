/*
 * NormalizedCut.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "NormalizedCut.hpp"
#include "IGraphPartition.hpp"
#include "ILinks.hpp"

double NormalizedCut::eval(IGraphPartition const & data) const {
	DoubleVector cut(data.nbObs(), 0);
	getCut(data, cut);
	double value(0);
	FOR_EACH_CONST( label ,data.used()) {
	value += cut[label] / data.degreeOfLabel(label);
}
	return value;
}

double NormalizedCut::eval(IGraphPartition const & data,
		size_t const & label) const {
	double cut(getCut(data, label));
	return cut / data.degreeOfLabel(label);
}

NormalizedCut::~NormalizedCut() {
}

Double2 NormalizedCut::getDelta2Shift(IGraphPartition const & data,
		size_t const & node, size_t const & newLabel,
		DoubleVector const & intra) const {
	Double2 delta(Cut::getDeltaShift(data, node, newLabel, intra));
	size_t const oldLabel(data.label(node));
	if (oldLabel != newLabel) {
		delta.first /= (data.sizeOfLabel(oldLabel) - data.graph().weight(node));
		delta.second /=
				(data.sizeOfLabel(newLabel) + data.graph().weight(node));
	}
	return delta;
}

double NormalizedCut::getDeltaShift(IGraphPartition const & data,
		size_t const & node, size_t const & newLabel,
		DoubleVector const & intra) const {
	Double2 delta(getDelta2Shift(data, node, newLabel, intra));
	return delta.first + delta.second;

}
