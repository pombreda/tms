#ifndef _TMS_COMMON_PROTOCOL__SIMPLE_REQUEST_PROCESSOR_HPP_
#define _TMS_COMMON_PROTOCOL__SIMPLE_REQUEST_PROCESSOR_HPP_
// boost
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
// common
#include <protocol/request_processor.hpp>
#include <protocol/message.hpp>
#include <protocol/server_exception.hpp>
#include <rtti/typeinfo.hpp>

namespace tms {
namespace common {
namespace protocol {
class SimpleRequestProcessor;
typedef boost::shared_ptr<SimpleRequestProcessor> SimpleRequestProcessorP;

typedef boost::function<MessageP (const Message&)> HandlerFunction;
typedef boost::unordered_map<rtti::TypeInfo, HandlerFunction> HandlersMap;
typedef boost::shared_ptr<HandlersMap> HandlersMapP;

class SimpleRequestProcessor : public RequestProcessor {
 public:
  explicit SimpleRequestProcessor(Server &server);
  SimpleRequestProcessor();
  RequestProcessorP Duplicate() const;
  MessageP Eval(const Message&)
      throw(ServerException);
  template<class Message>
  void AddHandler(boost::function<MessageP (const Message&)> handler)
      throw();
  template<class Message>
  void AddHandler(MessageP (*handler)(const Message&)) 
      throw();
 private:
  template<class Message>
  class FunctionWrapper;
  HandlersMap handlers_map_;
};

}
}
}
#include "simple_request_processor_impl.hpp"
#endif // _TMS_COMMON_PROTOCOL__SIMPLE_REQUEST_PROCESSOR_HPP_
