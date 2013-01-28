#ifndef REGISTERED_INSTANCE_HPP_
#define REGISTERED_INSTANCE_HPP_

#include "src/KMInstance.hpp"
#include "common.h"
#include "src/Partition.hpp"

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
	void out() const;
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
void RegisteredInstance::out() const {
	OUT<<"Instance name is "<<name<<"\n";
	OUT<<"Data were read from "<<Info::InstancesPath + dataName + ".dat"<<"\n";
	if (ctrsName.empty())
	OUT<<"No constraints were read\n";
	else
	OUT<<"Constraints were read from "<<Info::InstancesPath + ctrsName + ".ctrs"<<"\n";
	OUT<<"Number of observations : "<<nbObs() << std::endl;
	OUT<<"Number of attributes   : "<<nbAtt() << std::endl;
}
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

class ILauncher {
public:
	virtual void run(AvailableInstances id, size_t k)=0;
	virtual ~ILauncher() {
	}
};
template<AvailableInstances No> class RunAllFrom {
public:
	enum {
		value = No
	};

	template<class LaunchT>
	void go() {
		// on lance ce run
		LaunchT().run(No);
		// on lance le suivant
		RunAllFrom<static_cast<AvailableInstances const>(No + 1)> t;
		t.go<LaunchT>();
	}
};

// termination condition
template<> class RunAllFrom<AvailableInstances::SIZE> {
public:
	enum {
		value = AvailableInstances::SIZE
	};
	template<class LaunchT>
	void go() {

	}
};
#endif
