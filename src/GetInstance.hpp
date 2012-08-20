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
#define REGISTER_INSTANCE(x,y) x,
#include "instances.hxx"
#undef REGISTER_INSTANCE
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

#define REGISTER_INSTANCE(x,y) case x:dataName =#y;ctrsName = #x;break;
#include "instances.hxx"
#undef REGISTER_INSTANCE
	default:
		assert("UN_KONW INSTANCE"&&false);
		break;
	}
	name = ctrsName;
	if (ctrsName == dataName)
		ctrsName = "";
}

#endif
