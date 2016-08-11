#ifndef REGISTERED_MODULARITY_B_INSTANCE_HPP_
#define REGISTERED_MODULARITY_B_INSTANCE_HPP_

#include "BipartiteGraph.h"
#include "gencol.h"

enum AvailableModularityBInstances {
#define REGISTER_INSTANCE(x) x,
#include "RegisteredModularityBInstance.hxx"
#undef REGISTER_INSTANCE
  AvailableModularityBInstancesSize
};

class RegisteredModularityBInstance : public BipartiteGraph {
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

#endif

