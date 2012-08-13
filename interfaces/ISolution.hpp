/*
 * IProblem.hpp
 *
 *  Created on: 12 août 2012
 *      Author: manuel
 */

#ifndef I_SOLUTION_HPP_
#define I_SOLUTION_HPP_

#include "../utils/common.h"
#include "../interfaces/Problem.hpp"
#include "../interfaces/IPartition.hpp"
#include "../interfaces/ILinks.hpp"

/*
 * a templatiser sur partition et problem
 */
class ISolution {
public:
	Problem const & problem() const;
	ILinks const & links() const;
	ICriterion const & criterion() const;
	IPartition & partition();
	IPartition const & partition() const;
	ISolution(Problem const & problem, IPartition & partition);
	virtual ~ISolution();

	Double sumOfLabel(size_t l) const;
	Double & sumOfLabel(size_t l);

	Double score(size_t l) const;
	Double & score(size_t l);

	DoubleVector const & scores() const;
	DoubleVector & scores();

	virtual void shift(size_t, size_t)=0;
	virtual Double deltaShift(size_t n, size_t l) const = 0;
private:
	Problem const & _problem;
	IPartition & _partition;
	//
	DoubleVector _sumOfLabels;
	// score of each labels
	DoubleVector _scores;

};

inline ISolution::ISolution(Problem const & problem, IPartition & p) :
		_problem(problem), _partition(p), _sumOfLabels(problem.links().nbObs(),
				0) {

}
inline ILinks const & ISolution::links() const {
	return _problem.links();
}

inline ICriterion const & ISolution::criterion() const {
	return _problem.criterion();
}
inline ISolution::~ISolution() {

}

inline Double ISolution::sumOfLabel(size_t l) const {
	return _sumOfLabels[l];
}
inline Double & ISolution::sumOfLabel(size_t l) {
	return _sumOfLabels[l];
}
IPartition & ISolution::partition() {
	return _partition;
}
IPartition const & ISolution::partition() const {
	return _partition;
}
Double ISolution::score(size_t l) const {
	return _scores[l];
}
Double & ISolution::score(size_t l) {
	return _scores[l];
}

DoubleVector const & ISolution::scores() const {
	return _scores;
}
DoubleVector & ISolution::scores() {
	return _scores;
}
#endif /* IPROBLEM_HPP_ */
