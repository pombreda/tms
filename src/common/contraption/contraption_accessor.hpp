#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_ACCESSOR_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_ACCESSOR_HPP_
#include <contraption/contraption.hpp>
#include "contraption_accessor_fwd.hpp"
namespace tms {
namespace common {
namespace contraption {

class ContraptionAccessor {
 public:
  ContraptionAccessor(Contraption *contraption) 
      throw() :
      contraption_(contraption) { }
  boost::scoped_array<boost::scoped_ptr<FieldType> > &values() const
      throw() { return contraption_->values_; }
  boost::intrusive_ptr<Model>& model() const
      throw() { return contraption_->model_; }
  ContraptionID& id() const
      throw() { return contraption_->id_; }
 private:
  Contraption *contraption_;
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_ACCESSOR_HPP_
