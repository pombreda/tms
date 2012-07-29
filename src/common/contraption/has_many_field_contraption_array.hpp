#ifndef _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <vector>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/signals/connection.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
// common
#include <contraption/contraption.hpp>
#include <contraption/contraption_array.hpp>
#include <contraption/field/simple_field.hpp>

namespace tms {
namespace common {
namespace contraption {
// ToDo tests!!!
class HasManyFieldContraptionArray : public ContraptionArray {
 public:
  virtual void Save();
  friend class HasManyFieldImpl;
  virtual void Refresh();
 private:
  HasManyFieldContraptionArray(
      ModelP model,
      ModelP through_model,
      const IntField *id_column,
      const IntField *other_id_column,
      ContraptionID id)
      throw();
  ModelP through_model_;
  const IntField *id_column_;
  const IntField *other_id_column_;
  ContraptionID id_;
};

typedef boost::shared_ptr<HasManyFieldContraptionArray> HasManyFieldContraptionArrayP;

}
}
}

#endif // _TMS_COMMON_CONTRAPTION__HAS_MANY_FIELD_CONTRAPTION_ARRAY_HPP_
