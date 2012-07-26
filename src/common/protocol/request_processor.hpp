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
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace protocol {
class RequestProcessor;
typedef boost::shared_ptr<RequestProcessor> RequestProcessorP;

typedef boost::function<MessageP (const Message&)> HandlerFunction;
typedef boost::unordered_map<rtti::TypeInfo, HandlerFunction> HandlersMap;
typedef boost::shared_ptr<HandlersMap> HandlersMapP;

class RequestProcessor {
 public:
  virtual RequestProcessorP Duplicate() const = 0;
  virtual MessageP Eval(const Message&) = 0;
  virtual ~RequestProcessor() {}
 protected:
  static log4cplus::Logger logger_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__REQUEST_PROCESSOR_HPP_
