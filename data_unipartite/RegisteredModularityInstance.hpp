#ifndef REGISTERED_MODULARITY_INSTANCE_HPP_
#define REGISTERED_MODULARITY_INSTANCE_HPP_

#include "common.h"
#include "UnipartieInstance.h"

enum AvailableModularityInstances {
#define REGISTER_INSTANCE(x) x,
#include "RegisteredModularityInstance.hxx"
#undef REGISTER_INSTANCE
	AvailableModularityInstancesSize
};

//class RegisteredModularityInstance: public UnipartieInstance {
//public:
//	static std::string InstancesPath;
//public:
//	void setName(AvailableModularityInstances id);
//public:
//	std::string name;
//public:
//	void out() const;
//private:
//	std::string dataName;
//public:
//	RegisteredModularityInstance(AvailableModularityInstances id);
//	virtual ~RegisteredModularityInstance() {
//	}
//	void exportAmpl(std::string const & fileName);
//};
//std::string RegisteredModularityInstance::InstancesPath =
//		"../modularity_instances/";
//void RegisteredModularityInstance::out() const {
//	OUT<<"Instance name is "<<name<<"\n";
//	OUT<<"Data were read from "<<InstancesPath + dataName + ".graph"<<"\n";
//}
//inline RegisteredModularityInstance::RegisteredModularityInstance(
//		AvailableModularityInstances id) {
//	setName(id);
//	read(InstancesPath + dataName + ".graph");
//}
//
//inline void RegisteredModularityInstance::setName(
//		AvailableModularityInstances id) {
//	switch (id) {
//#define REGISTER_INSTANCE(x) case x:dataName = #x;break;
//#include "RegisteredModularityInstance.hxx"
//#undef REGISTER_INSTANCE
//	default:
//		std::cout << id << std::endl;
//		assert("UN_KONW INSTANCE" && false);
//		break;
//	}
//	name = dataName;
//}
//void RegisteredModularityInstance::exportAmpl(std::string const & fileName) {
//	std::ofstream file(fileName.c_str());
//	file << "param N := " << nbNodes() << ";" << std::endl;
//	file << "set E := " << std::endl;
//	for (size_t i(0); i < nbNodes(); ++i) {
//		for (auto const & j : (*this)(i)) {
//			if (i < j.first) {
//				file << std::setw(6) << 1 + i;
//				file << std::setw(6) << 1 + j.first;
//				file << std::endl;
//			}
//		}
//	}
//	file << ";" << std::endl;
//	file.close();
//}
#endif

