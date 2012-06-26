#ifndef _TMS_COMMON_CONTRAPTION__MODEL_CATALOG_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_CATALOG_HPP_
#include <typeinfo>
namespace tms {
namespace common {
namespace contraption {

class ModelCatalog {
 public:
  void Register(Model *model) {
    const std::string name = typeid(model).name();
    if (catalog_.count(name) != 0)  {
      throw exception();//oops
    }
    catalog_[name] = model;
  }

  bool Registered(Model *model) const {
    const std::string name = typeid(model).name();
    std::map<std::string, Model*>::iterator it = catalog_.find(name);
    if (it == catalog_.end())  {
      return false;
    }
    return typeinfo(it->second) == typeinfo(model);
  }

  static Instance() {
    if (!instance_) {
      instance_ = new ModelCatalog();
    }
    return instance_;
  }

  Model *GetModel(const std::string &name) const {
    std::map<std::string, Model*>::iterator it = catalog_.find(name);
    if (it == catalog_.end())  {
      throw exception();//oops
    }    
    return it->second;
  }
 private:
  ModelCatalog(){}
  static ModelCatalog *instance_;
  std::map<std::string, Model*> catalog_
};

ModelCatalog::instance_ = 0;

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MODEL_CATALOG_HPP_
