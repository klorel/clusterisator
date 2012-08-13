/*
 * IProblem.hpp
 *
 *  Created on: 12 août 2012
 *      Author: manuel
 */

#ifndef PROBLEM_HPP_
#define PROBLEM_HPP_

#include "../utils/common.h"

/*
 * a templatiser sur ILinks et ICriterion
 */

class Problem {
public:
	ILinks const & links() const;
	ICriterion const & criterion() const;

	Problem(ILinks const &, ICriterion const &);
	virtual ~Problem();
private:
	ILinks const & _iLink;
	ICriterion const & _iCriterion;

};

inline Problem::Problem(ILinks const & iLinks, ICriterion const & iCriterion) :
		_iLink(iLinks), _iCriterion(iCriterion) {

}

inline Problem::~Problem() {

}

#endif /* IPROBLEM_HPP_ */
