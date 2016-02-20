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

class RegisteredModularityInstance: public UnipartieInstance {
public:
	static std::string InstancesPath;
public:
	void setName(AvailableModularityInstances id);
public:
	std::string _name;
public:
	void out(std::ostream & stream = std::cout) const;
private:
	std::string _dataName;
public:
	virtual std::string name() const;
	RegisteredModularityInstance(AvailableModularityInstances id);
	virtual ~RegisteredModularityInstance() {
	}
	void exportAmpl(std::string const & fileName);
};
inline std::string RegisteredModularityInstance::name() const {
	return _name;
}
inline void RegisteredModularityInstance::out(std::ostream & stream) const {
	stream << "Instance name is " << _name << std::endl;
	stream << "Data were read from " << InstancesPath + _dataName + ".txt"
			<< std::endl;
}
inline RegisteredModularityInstance::RegisteredModularityInstance(
		AvailableModularityInstances id) {
	setName(id);
	edges().read(InstancesPath + _dataName + ".txt");
	build();
}

inline void RegisteredModularityInstance::setName(
		AvailableModularityInstances id) {
	switch (id) {
#define REGISTER_INSTANCE(x) case x:_dataName = #x;break;
#include "RegisteredModularityInstance.hxx"
#undef REGISTER_INSTANCE
	default:
		std::cout << id << std::endl;
		assert("UNKNOWN INSTANCE" && false);
		break;
	}
	_name = _dataName;
}
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

