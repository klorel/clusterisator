/*
 * KMeansAlgo.cpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#include "KMAlgo.hpp"

std::string const KMAlgo::HMEANS = "H-MEANS";
std::string const KMAlgo::KMEANS = "K-MEANS";

IntVector KMAlgo::_Buffer = IntVector();

Double KMAlgo::ComputeMssc(IPartition const & x, KMInstance const & instance) {
	RectMatrix centers(x.maxNbLabels(), instance.nbAtt());
	centers.assign(0);
	DoubleVector weights(x.maxNbLabels(), Zero<Double>());
	for (auto const & l : x.used()) {
		for (auto const & i : x.list(l)) {
			weights[l] += instance.weight(i);
			for (size_t d(0); d < instance.nbAtt(); ++d)
				centers.plus(l, d, instance.get(i, d) * instance.weight(i));
		}
	}
	Double result(Zero<Double>());
	for (size_t i(0); i < instance.nbObs(); ++i) {
		size_t const l(x.label(i));
		for (size_t d(0); d < instance.nbAtt(); ++d)
			result += instance.weight(i)
					* std::pow(
							instance.get(i, d) - centers.get(l, d) / weights[l],
							2);

	}

	return result;
}

void Input::out(std::string const & name) const {

	OUT<< std::setw(10) << name;
	OUT << std::setw(10) << _timer.elapsed();
	OUT << std::setw(10) << _ite;
	OUT << std::setw(20) << std::setprecision(10) << _partition.cost();
	OUT << "\n";
}
void Input::headers() {
	OUT<< std::setw(10) << "ALGO";
	OUT<< std::setw(10) << "TIME";
	OUT<< std::setw(10) << "ITERATION";
	OUT<< std::setw(20) << "COST";
	OUT<< "\n";
}
KMPartition & Input::partition() {
	return _partition;
}
KMPartition const& Input::partition() const {
	return _partition;
}
Double Input::cost() const {
	return _partition.cost();

}
size_t & Input::ite() {
	return _ite;
}
size_t Input::ite() const {
	return _ite;
}
