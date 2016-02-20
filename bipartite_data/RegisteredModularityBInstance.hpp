#ifndef REGISTERED_MODULARITY_B_INSTANCE_HPP_
#define REGISTERED_MODULARITY_B_INSTANCE_HPP_

#include "gencol.h"
#include "BipartiteGraph.hpp"

enum AvailableModularityBInstances {
#define REGISTER_INSTANCE(x) x,
#include "RegisteredModularityBInstance.hxx"
#undef REGISTER_INSTANCE
	AvailableModularityBInstancesSize
};

class RegisteredModularityBInstance: public BipartiteGraph {
public:
	static std::string InstancesPath;
public:
	void setName(AvailableModularityBInstances id);
public:
	std::string _name;
public:
	void out(std::ostream & stream = std::cout) const;
private:
	std::string _dataName;
public:
	virtual std::string name() const;
	RegisteredModularityBInstance(AvailableModularityBInstances id);
	virtual ~RegisteredModularityBInstance() {
	}
	void exportAmpl(std::string const & fileName);
};
inline std::string RegisteredModularityBInstance::name() const {
	return _name;
}
inline void RegisteredModularityBInstance::out(std::ostream & stream) const {
	stream << "Instance name is " << _name << std::endl;
	stream << "Data were read from " << InstancesPath + _dataName + ".txt"
			<< std::endl;
}
inline RegisteredModularityBInstance::RegisteredModularityBInstance(
		AvailableModularityBInstances id) {
	setName(id);
	edges().read(InstancesPath + _dataName + ".txt");
	build();
}

inline void RegisteredModularityBInstance::setName(
		AvailableModularityBInstances id) {
	switch (id) {
#define REGISTER_INSTANCE(x) case x:_dataName = #x;break;
#include "RegisteredModularityBInstance.hxx"
#undef REGISTER_INSTANCE
	default:
		std::cout << id << std::endl;
		assert("UN_KONW INSTANCE" && false);
		break;
	}
	_name = _dataName;
}
//
//class ILauncher {
//public:
//	virtual void run(AvailableInstances id, size_t k)=0;
//	virtual ~ILauncher() {
//	}
//};
//template<AvailableInstances No> class RunAllFrom {
//public:
//	enum {
//		value = No
//	};
//
//	template<class LaunchT>
//	void go() {
//		// on lance ce run
//		LaunchT().run(No);
//		// on lance le suivant
//		RunAllFrom<static_cast<AvailableInstances const>(No + 1)> t;
//		t.go<LaunchT>();
//	}
//};
//
//// termination condition
//template<> class RunAllFrom<AvailableInstances::SIZE> {
//public:
//	enum {
//		value = AvailableInstances::SIZE
//	};
//	template<class LaunchT>
//	void go() {
//
//	}
//};

inline void RegisteredModularityBInstance::exportAmpl(
		std::string const & fileName) {
	//std::ofstream file(fileName.c_str());
	//file << "param N := "<<nbNodes() << ";" << std::endl;
	//file << "set E := "<<std::endl;
	//for(size_t i(0); i<nbNodes(); ++i){
	//	for(auto const & j : (*this)(i)){
	//		if(i<j.first){
	//			file << std::setw(6)<<1+i;
	//			file << std::setw(6)<<1+j.first;
	//			file << std::endl;
	//		}
	//	}
	//}
	//file << ";"<<std::endl;
	//file.close();
}

#endif

