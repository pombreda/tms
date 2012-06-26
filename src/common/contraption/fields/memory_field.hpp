#ifndef _TMS_COMMON_CONTRAPTION__MEMORY_FIELD_HPP_
#define _TMS_COMMON_CONTRAPTION__MEMORY_FIELD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <contraption/field.hpp>
namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// MemoryField class.
//------------------------------------------------------------
template<class T>
class MemoryField :: public Field<T> {
 public:
  MemoryField(const std::string &name) : FieldT<T>(name) {}
  virtual void* GetValue(Model *model, ContraptionID id) const {
    // model must be derived from MemoryModel or we are doing
    // something wrong
    MemoryModel *memory_model = dynamic_cast<MemoryModel*>(model);
    if (!memory_model) {
      throw exception();//oops
    }
    const std::map<std::string, void*> &memory = memory_model->memory_[id];
    std::map<std::string, void*>::iterator it = memory.find(name);
    if (it == memory.end()) {
      throw exception();//oops
    }
    void *ret = it->second;
    if (!ret) {
      ret = type()->New();
    }
    return ret;
  }

  virtual void SetValue(Model *model, ContraptionID id, void *value) const {
    // model must be derived from MemoryModel or we are doing
    // something wrong
    MemoryModel *memory_model = dynamic_cast<MemoryModel*>(model);
    if (!memory_model) {
      throw exception();//oops
    }
    const std::map<std::string, void*> &memory = memory_model->memory_[id];
    std::map<std::string, void*>::iterator it = memory.find(name);
    if (it == memory.end()) {
      throw exception();//oops
    }
    it->second = value;
  }
};

}
}
}
#endif // _TMS_COMMON_CONTRAPTION__MEMORY_FIELD_HPP_
