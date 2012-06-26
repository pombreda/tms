#ifndef _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_HPP_
#define _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <contraption/model.hpp>
namespace tms {
namespace common {
namespace contraption {

//------------------------------------------------------------
// MemoryModel class. 
//------------------------------------------------------------
class MemoryModel : virtual public Model {
 public:
  friend void* MemoryField::GetValue(Model *model, ContraptionID id) const;
 protected:
  MemoryModel() : memory_(0) {}
  void AddField(MemoryField *field) {
    Model::AddField(field);
  }
 private:
  std::vector< std::map<std::string, void*> > memory_;
}
}
}
}

#endif // _TMS_COMMON_CONTRAPTION__MEMORY_MODEL_HPP_
