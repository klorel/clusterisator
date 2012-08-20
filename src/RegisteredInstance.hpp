#ifndef REGISTERED_INSTANCE_HPP_
#define REGISTERED_INSTANCE_HPP_

#include "../src/KMInstance.hpp"
#include "common.h"
#include "../src/Partition.hpp"

enum AvailableInstances {
#define REGISTER_INSTANCE(x,y) x,
#include "RegisteredInstance.hxx"
#undef REGISTER_INSTANCE
	SIZE
};

class RegisteredInstance: public KMInstance {
public:
	void real(Partition &) const;
	Partition real() const;
public:
	void setNames(AvailableInstances id);
public:
	std::string name;
private:
	std::string dataName;
	std::string ctrsName;
public:
	RegisteredInstance(AvailableInstances id);
	virtual ~RegisteredInstance() {
	}

};
class Info {
public:
	static std::string const InstancesPath;
};

std::string const Info::InstancesPath = "../instances/";

inline RegisteredInstance::RegisteredInstance(AvailableInstances id) {
	setNames(id);
	readData(Info::InstancesPath + dataName + ".dat");
	if (!ctrsName.empty()) {
		readConstraints(Info::InstancesPath + ctrsName + ".ctrs");
	}
}
inline void RegisteredInstance::real(Partition & result) const {
	(Info::InstancesPath + dataName + ".real") >> result;
}
inline Partition RegisteredInstance::real() const {
	Partition result(nbObs());
	real(result);
	return result;
}

inline void RegisteredInstance::setNames(AvailableInstances id) {
	switch (id) {
#define REGISTER_INSTANCE(x,y) case x:dataName =#y;ctrsName = #x;break;
#include "RegisteredInstance.hxx"
#undef REGISTER_INSTANCE
	default:
		std::cout << id << std::endl;
		assert("UN_KONW INSTANCE"&&false);
		break;
	}
	name = ctrsName;
	if (ctrsName == dataName)
		ctrsName = "";
}

#endif
