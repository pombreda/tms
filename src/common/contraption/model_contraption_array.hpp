#ifndef _TMS_COMMON_CONTRAPTION__MODEL_CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_CONTRAPTION_ARRAY_HPP_
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

namespace tms {
namespace common {
namespace contraption {
// ToDo tests!!!
class ModelContraptionArray : public ContraptionArray {
 public:
  virtual void Save();
  ModelP model() {return model_;}
  friend class Model;

  virtual void Refresh();
  void push_back(const ContraptionP& contraption);

 private:
  ModelContraptionArray(
      const std::vector<ContraptionP> &contraptions,
      ModelP model)
      throw();
};

typedef boost::shared_ptr<ModelContraptionArray> ModelContraptionArrayP;

}
}
}

#endif // _TMS_COMMON_CONTRAPTION__MODEL_CONTRAPTION_ARRAY_HPP_
