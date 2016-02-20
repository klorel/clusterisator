/*
 * Partition.cpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#include "Partition.hpp"

Partition::Partition(size_t nbObs, size_t nbMaxLabels) {
	oneLabel(nbObs, nbMaxLabels);
}
Partition::Partition(Partition const & rhs) {
	oneLabel(rhs.nbObs(), rhs.maxNbLabels());
	for (size_t n(0); n < nbObs(); ++n)
		shift(n, rhs.label(n));
}

Partition & Partition::operator=(Partition const & rhs) {
	if (this != &rhs) {
		if (rhs.nbObs() != nbObs() || rhs.maxNbLabels() != maxNbLabels()) {
			oneLabel(rhs.nbObs(), rhs.maxNbLabels());
		}
		for (size_t n(0); n < nbObs(); ++n)
			shift(n, rhs.label(n));
	}
	return *this;

}

Partition::~Partition() {

}

size_t Partition::fusion(size_t const & label1, size_t const & label2) {
	assert(label1 < maxNbLabels());
	assert(label2 < maxNbLabels());

	if (label1 == label2) {
		return label1;
	}

	//Move obs from the smallest into the biggest label, to minimize movements
	size_t destLabel, origLabel;
	if (label1 < label2) {
		destLabel = label1;
		origLabel = label2;
	} else {
		destLabel = label2;
		origLabel = label1;
	}

	//Once we'll start to actually move observation, we won't be able to iterate
	//on this list anymore (because it will be modified) and start by copying it
	IntList obsToMove = observations(origLabel);
	for (auto obs : obsToMove) {
		shift(obs, destLabel);
	}

	return destLabel;
}

void Partition::oneLabel(size_t nbObs, size_t nbMaxLabels) {
	_labelLists.assign(nbMaxLabels, IntList());
	_size.assign(nbMaxLabels, 0);
	_usedLabels.reset(nbMaxLabels);
	_unUsedLabels.reset(nbMaxLabels);
	_labelWeights.assign(nbMaxLabels, 0);
	_labelWeights[0] = (Double) nbObs;

	_nodePosition.assign(nbObs, _labelLists[0].end());
	_nodeWeights.assign(nbObs, 1);
	_labels.assign(nbObs, 0);

	_usedLabels.clear();
	_usedLabels.insert(0);

	_unUsedLabels.fill();
	_unUsedLabels.erase(0);

	for (size_t n(0); n < this->nbObs(); ++n) {
		_labelLists[0].push_front(n);
		_nodePosition[n] = _labelLists[0].begin();
	}
	_size[0] = nbObs;
}

void Partition::setLabels(IntVector const & labels) {
	assert(labels.size() == nbObs());

	for (size_t i(0); i < labels.size(); ++i) {
		assert(
				labels[i] < maxNbLabels()
						&& "you must provide labels l : 0<=l<maxNbLabels()");
		shift(i, labels[i]);
	}

}
void Partition::setLabels(Partition const & rhs) {
	setLabels(rhs._labels);
}
bool Partition::shift(size_t observation, size_t to) {
	assert(observation < nbObs());

	size_t const from(_labels[observation]);
	if (from != to) {
		--_size[from];
		_labelLists[from].erase(_nodePosition[observation]);
		_labelWeights[from] -= _nodeWeights[observation];

		if (_size[from] == 0) {
			_usedLabels.erase(from);
			_unUsedLabels.insert(from);
		}
		if (_size[to] == 0) {
			_unUsedLabels.erase(to);
			_usedLabels.insert(to);
		}
		++_size[to];
		_labelLists[to].push_front(observation);
		_labelWeights[to] += _nodeWeights[observation];
		_nodePosition[observation] = _labelLists[to].begin();
		_labels[observation] = to;
	}
	return true;
}

bool Partition::checkLists() const {
	for (auto const & l : usedLabels()) {
		if (sizeOfLabel(l) == 0) {
			std::cout << "error on size of label " << l << "\n";
		}
		for (auto const & n : observations(l)) {
			if (n != *_nodePosition[n]) {
				std::cout << n << " iterator in label " << l << " is wrong\n";
			}
		}
	}
	return true;
}

bool Partition::checkWeights() const {
	for (auto const & l : usedLabels()) {
		Double w(0);
		for (auto const & n : _labelLists[l])
			w += obsWeight(n);
		if (!IsEqual(w, labelWeight(l))) {
			std::cout << "wrong label weight of " << l << " : ";
			std::cout << labelWeight(l) << " != " << w << "\n";
			return false;
		}
	}
	return true;
}

void Partition::random() {
	IndexedList nodes(nbObs(), true);
	for (auto const & n : nodes)
		shift(n, Number::Generator() % maxNbLabels());
}
void Partition::random(size_t k) {
	assert(k < maxNbLabels());
	assert(nbObs() > 0);
	IndexedList nodes(nbObs(), true);
	for (size_t i(0); i < (k == 0 ? maxNbLabels() : k); ++i) {
		size_t const n(nodes.pop_random());
//		OUT<< n << " "<<i<<"\n";
		shift(n, i);
	}
	for (auto const & n : nodes)
		shift(n, Number::Generator() % (k == 0 ? maxNbLabels() : k));
}

void Partition::setWeights(DoubleVector const & weights) {
	assert(weights.size() == nbObs());

	_nodeWeights = weights;
	_labelWeights.assign(maxNbLabels(), 0);
	for (size_t i(0); i < nbObs(); ++i)
		_labelWeights[_labels[i]] += _nodeWeights[i];
}
