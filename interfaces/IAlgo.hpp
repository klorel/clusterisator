/*
 * IAlgo.hpp
 *
 *  Created on: 4 août 2012
 *      Author: manuel
 */

#ifndef IALGO_HPP_
#define IALGO_HPP_

class IPartition;
class ICriterion;
class IAlgo {
public:
	virtual void run(IPartition const & input, IPartition & output) = 0;
public:
	ICriterion const & criterion();
	IAlgo(ICriterion const & criterion);
	virtual ~IAlgo();

private:
	ICriterion const & _criterion;
};

inline ICriterion const & IAlgo::criterion() {
	return _criterion;
}

IAlgo::~IAlgo() {
}

IAlgo::IAlgo(ICriterion const & criterion) :
		_criterion(criterion) {

}

#endif /* IALGO_HPP_ */
