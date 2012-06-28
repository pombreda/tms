#ifndef _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
#define _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
// std
#include <vector>
// boost
#include <boost/signal.hpp>
// common
#include <contraption/contraption.hpp>
#include "contraption_array_fwd.hpp"

namespace tms {
namespace common {
namespace contraption {
//------------------------------------------------------------
// ContraptionArray class. ToDo: rewrite to check model
// correctness. Now it'll report about incorrect model
// only on Save() attempt.
//------------------------------------------------------------
class ContraptionArray : public std::vector<ContraptionP> {
 public:
  void Save();
  friend class Model;
 private:
  typedef boost::signal<void (const std::vector<ContraptionP>&, 
                              const std::vector<ContraptionP>&)> SaverType;

  ContraptionArray(
      std::auto_ptr<SaverType> saver,
      const std::vector<ContraptionP> &contraptions) 
      throw();
  std::auto_ptr<SaverType> saver_;
  vector<ContraptionP> back_up_;
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__CONTRAPTION_ARRAY_HPP_
