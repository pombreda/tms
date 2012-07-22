#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <vector>
// boost
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
// common
#include <contraption/contraption.hpp>
#include "contraption_array_fwd.hpp"

#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Weffc++"
//ToDo rewrite
namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// ContraptionArray class. ToDo: rewrite to check model
// correctness. Now it'll report about incorrect model
// only on Save() attempt.
//------------------------------------------------------------
class ContraptionArray : private std::vector<ContraptionP> {
 public:
  void Save();
  ModelP model() {return model_;}
  friend class Model;

  void push_back(const ContraptionP& contraption);
  void erase(size_t position);
  ContraptionP& at(size_t position);
  size_t size();

  void SetOnChange(boost::function<void ()> f);

 private:
  typedef boost::signal<void (const std::vector<ContraptionP>&,
                              const std::vector<ContraptionP>&)> SaverType;

  ContraptionArray(
      std::auto_ptr<SaverType> saver,
      const std::vector<ContraptionP> &contraptions,
      ModelP model)
      throw();
  void OnChange();
  std::auto_ptr<SaverType> saver_;
  vector<ContraptionP> back_up_;
  ModelP model_;
  boost::signal<void ()>  on_change_;
};
}
}
}
#pragma GCC diagnostic warning "-Wnon-virtual-dtor"
#pragma GCC diagnostic warning "-Weffc++"

#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
