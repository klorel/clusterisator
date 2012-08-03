/*
 * MultiStart.cpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#include "MultiStart.hpp"

MultiStart::MultiStart(Data & rhs, ICriterion const & criterion) :
		_data(rhs), _criterion(criterion) {
	_labels.reset(rhs.nbNodes());
	_nodes.reset(rhs.nbNodes());
}

MultiStart::~MultiStart() {

}

int & MultiStart::nbEmptyLabel() {
	return _nbEmptyLabel;
}

double const & MultiStart::score() const {
	return _score;
}
void MultiStart::init() {
	_labels.fill();
	_nodes.fill();

}

void MultiStart::setLabels() {
	_labels.clear();
	for (size_t const & l : _data.used())
		_labels.insert(l);
	if (_nbEmptyLabel > 0) {
		_expectedSize = static_cast<size_t>(ceil(
				_data.nbLabels() * (100 + _nbEmptyLabel) / 100.0));
		for (auto const & l : _data.unUsed()) {
			_labels.insert(l);
			if (_labels.size() > _expectedSize)
				break;
		}
	}

}

bool MultiStart::run(size_t const & n) {
	_reference = _data.nodeLabel();

	_cst = 0;
	_score = 0;
	for (auto const & l : _data.used()) {
//		std::cout << l << "\n";
		double const v(_criterion.eval(_data, l));
		_score += v;
		if (!_labels.contains(l))
			_cst += v;
	}
	if (_nbEmptyLabel > 0)
		setLabels();

	size_t ite(0);
	bool success(false);
	while (ite < n) {
		++ite;
		assert(fabs(_score-_criterion.eval(_data))<1e-10);
		if (generate()) {
//			std::cout << std::setw(8) << ite;
//			std::cout << std::setw(8) << _data.nbLabels();
//			std::cout << std::setw(8) << _labels.size();
//			std::cout << std::setw(15) << _score;
//			std::cout << "\n";
			ite = 0;
			success = true;
			if (_nbEmptyLabel > 0)
				setLabels();
		}
	}
	return success;
}

bool MultiStart::generate() {
	for (auto const & n : _nodes) {
		size_t const l(_labels.random());
//		std::cout << n << " : " << _data.labelOfNode(n) << " --> " << l << "\n";
		_data.shift(n, l);
	}
	double score(_cst);
	for (auto const & l : _labels)
		score += _criterion.eval(_data, l);

//	assert(fabs(score-criterion.Eval(_data))<1e-10);
	bool success(_criterion.isBetter(score, _score));
//	std::cout << score << "\n";
//	exit(0);
	if (success) {
		_score = score;
		for (auto const & n : _nodes)
			_reference[n] = _data.labelOfNode(n);
	} else {
		for (auto const & n : _nodes) {
			_data.shift(n, _reference[n]);
		}
	}
	return success;

}
