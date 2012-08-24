/*
 * KMeansAlgo.hpp
 *
 *  Created on: 11 août 2012
 *      Author: manuel
 */

#ifndef KMEANSALGO_HPP_
#define KMEANSALGO_HPP_

#include "src/common.h"
#include "src/KMInput.hpp"
#include "src/KMeans.hpp"
#include "src/HMeans.hpp"

class KMAlgo {
public:
	static Double ComputeMssc(IPartition const &, KMInstance const &);

};

template<bool isTraceOn> class HKMeans {
public:
	bool operator()(KMInput & input) {
		bool success(false);
		if (_hMeans(input))
			success = true;
		if (_kMeans(input))
			success = true;
		return success;
	}
private:
	HMeans<isTraceOn> _hMeans;
	KMeans<isTraceOn> _kMeans;
};
#endif /* KMEANSALGO_HPP_ */

