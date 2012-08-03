/*
 * MultiStart.hpp
 *
 *  Created on: 19 juil. 2012
 *      Author: manuel
 */

#ifndef MULTISTART_HPP_
#define MULTISTART_HPP_

/*
 * on tire aléatoirement à partir d'une solution courante
 */
#include "common.h"
#include "IndexedList.hpp"
#include "Data.hpp"
#include "ICriterion.hpp"

class MultiStart {
public:
	MultiStart(Data &, ICriterion const & criterion);
	virtual ~MultiStart();

	// on initialise à partir d'un collection de label
	template<class T> void init(T const &);
	void init();
	//
	int & nbEmptyLabel();
	double const & score() const;
	// retourne vrai si on a améliorer
	bool run( size_t const & n);
private:
	bool generate();
	void setLabels();
private:
	Data & _data;
	ICriterion const & _criterion;
	// pourcentage de label libre ajouter à la sélection
	int _nbEmptyLabel;
	//
	IntSet _initLabels;
	IndexedList _labels;
	IndexedList _nodes;
	size_t _expectedSize;
	double _cst;
	IntVector _reference;
	double _score;
};

template<class T> inline void MultiStart::init(T const & t) {
	_initLabels.clear();
	Insert(t, _initLabels);
	if (_nbEmptyLabel < 0) {
		_labels.fill();

	} else {
		setLabels();
	}
	_nodes.clear();
	for (auto const & l : _labels)
		Insert(_data.list(l), _nodes);

}
#endif /* MULTISTART_HPP_ */
