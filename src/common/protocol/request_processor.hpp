#ifndef _TMS_COMMON_PROTOCOL__REQUEST_PROCESSOR_HPP_
#define _TMS_COMMON_PROTOCOL__REQUEST_PROCESSOR_HPP_
// boost
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
// log4cplus
#include <log4cplus/logger.h>
// common
#include <protocol/message.hpp>
#include <protocol/server_exception.hpp>
#include <protocol/server_fwd.hpp>
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace protocol {
class RequestProcessor;
typedef boost::shared_ptr<RequestProcessor> RequestProcessorP;

class RequestProcessor {
 public:
  explicit RequestProcessor(Server &server_);
  RequestProcessor();
  virtual RequestProcessorP Duplicate() const = 0;
  virtual MessageP Eval(const Message&, Server &server) = 0;
  virtual ~RequestProcessor() {}

  Server& server() const
      throw();

  void set_server(Server &server) 
      throw();

 protected:
  static log4cplus::Logger logger_;
  Server *server_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__REQUEST_PROCESSOR_HPP_
