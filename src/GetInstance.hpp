/*
 * Wine.hpp
 *
 *  Created on: 18 août 2012
 *      Author: manuel
 */

#ifndef GET_INSTANCE_HPP_
#define GET_INSTANCE_HPP_

#include "../src/KMInstance.hpp"
#include "common.h"
#include "../src/Partition.hpp"

enum AvailableInstances {
	wine,
	wine1,
	wine2,
	iris,
	iris1,
	iris2,
//	synthetic,
	balance_scale,
	balance_scale1,
	balance_scale2,
	ionosphere,
	ionosphere1,
	ionosphere2,
	letter,
	magic,
	magic1,
	magic2,
	optdigits,
	optdigits1,
	optdigits2,
	page_blocks,
	page_blocks1,
	page_blocks2,
	satimage,
	satimage1,
	satimage2,

	soybean_small,
	yeast,
	yeast1,
	yeast2,
	SIZE
};

template<AvailableInstances instance>
class GetInstance: public KMInstance {
public:
	void real(Partition &) const;
	Partition real() const;
public:
	void setNames();
public:
	std::string name;
private:
	std::string dataName;
	std::string ctrsName;
public:
	GetInstance();
	virtual ~GetInstance() {
	}

};

std::string const InstancesPath = "../instances/";

template<AvailableInstances InstanceT> inline GetInstance<InstanceT>::GetInstance() {
	setNames();
	readData(InstancesPath + dataName + ".dat");
	if (!ctrsName.empty()) {
		readConstraints(InstancesPath + ctrsName + ".ctrs");
	}
}
template<AvailableInstances InstanceT> inline void GetInstance<InstanceT>::real(
		Partition & result) const {
	(InstancesPath + dataName + ".real") >> result;
}
template<AvailableInstances InstanceT> inline Partition GetInstance<InstanceT>::real() const {
	Partition result(nbObs());
	real(result);
	return result;
}

template<AvailableInstances InstanceT> inline void GetInstance<InstanceT>::setNames() {
	switch (InstanceT) {
	case wine:
		dataName = "wine";
		ctrsName = "";
		break;
	case wine1:
		dataName = "wine";
		ctrsName = "wine1";
		break;
	case wine2:
		dataName = "wine";
		ctrsName = "wine2";
		break;
		break;
	case balance_scale:
		dataName = "balance_scale";
		ctrsName = "";
		break;
	case balance_scale1:
		dataName = "balance_scale";
		ctrsName = "balance_scale1";
		break;
	case balance_scale2:
		dataName = "balance_scale";
		ctrsName = "balance_scale2";
		break;
	case ionosphere:
		dataName = "ionosphere";
		ctrsName = "";
		break;
	case ionosphere1:
		dataName = "ionosphere";
		ctrsName = "ionosphere2";
		break;
	case ionosphere2:
		dataName = "ionosphere";
		ctrsName = "ionosphere2";
		break;
	case iris:
		dataName = "iris";
		ctrsName = "";
		break;
	case iris1:
		dataName = "iris";
		ctrsName = "iris1";
		break;
	case iris2:
		dataName = "iris";
		ctrsName = "iris2";
		break;
	case magic:
		dataName = "magic";
		ctrsName = "";
		break;
	case magic1:
		dataName = "magic";
		ctrsName = "magic1";
		break;
	case magic2:
		dataName = "magic";
		ctrsName = "magic2";
		break;
	case optdigits:
		dataName = "optdigits";
		ctrsName = "";
		break;
	case optdigits1:
		dataName = "optdigits";
		ctrsName = "optdigits1";
		break;
	case optdigits2:
		dataName = "optdigits";
		ctrsName = "optdigits2";
		break;
	case letter:
		dataName = "letter";
		ctrsName = "";
		break;
	case page_blocks:
		dataName = "page_blocks";
		ctrsName = "";
		break;
	case page_blocks1:
		dataName = "page_blocks";
		ctrsName = "page_blocks1";
		break;
	case page_blocks2:
		dataName = "page_blocks";
		ctrsName = "page_blocks2";
		break;
	case satimage:
		dataName = "satimage";
		ctrsName = "";
		break;
	case satimage1:
		dataName = "satimage";
		ctrsName = "satimage1";
		break;
	case satimage2:
		dataName = "satimage";
		ctrsName = "satimage2";
		break;
//	case synthetic:
//		dataName = "synthetic";
//		ctrsName = "";
//		break;
	case yeast:
		dataName = "yeast";
		ctrsName = "";
		break;
	case yeast1:
		dataName = "yeast";
		ctrsName = "yeast1";
		break;
	case yeast2:
		dataName = "yeast";
		ctrsName = "yeast2";
		break;
		break;
	case soybean_small:
		dataName = "soybean_small";
		ctrsName = "";
		break;
	default:
		assert("UN_KONW INSTANCE"&&false);
		break;
	}
	if (ctrsName.empty())
		name = dataName;
	else
		name = ctrsName;
}

#endif
