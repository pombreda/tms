#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
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
class ContraptionArray : protected std::vector<ContraptionP> {
 public:
  virtual void Save() = 0;
  ModelP model() {return model_;}

  virtual void Refresh() = 0;
  void push_back(ContraptionP contraption);
  void erase(size_t position);
  ContraptionP& at(size_t position);
  size_t size();

  void SetOnChange(boost::function<void ()> f);

 protected:
  boost::signal<void ()>  on_change_;
  ModelP model_;  
  ContraptionArray(
      const std::vector<ContraptionP> &contraptions,
      ModelP model)
      throw();  
  void Init();
  std::vector<ContraptionP> to_remove_;
  void OnChange();
 private:
  typedef boost::signals::scoped_connection SlotConnection;
  typedef boost::shared_ptr<SlotConnection> SlotConnectionP;
  std::vector<SlotConnectionP> connections_;
};
}
}
}

#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
