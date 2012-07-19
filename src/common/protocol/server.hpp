#ifndef _TMS_COMMON_PROTOCOL__SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__SERVER_HPP_

// std
#include <istream>
// boost 
#include <boost/signals.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
// common
#include <protocol/protocol.hpp>
namespace tms {
namespace protocol {
namespace commont {

class Server {
 public:
  Server(std::iostream &stream, ProtocolP protocol)
      throw() :
      running_(false),
      stream_(stream),
      protocol_(protocol) {}  

  void Listen() {
    try {
      listen_thread_.reset(new boost::thread(
          bind(&Server::Listen_Thread_, this)));
      running_ = true;
    } catch(boost::thread_resource_error &e) {
      throw ServerException(&e);
    }
  }

  void Stop() 
      throw() {
    running_ = false;
    try {
      listen_thread_->join();
    } catch (boost::thread_interrupted  e) {
      throw ServerException(&e);
    }
  }

  bool IsListening() 
      throw() {  
    return running_;
  }

  virtual ~Server() 
      throw() {}
 private:
  virtual void ListenThread()
      throw () {
    while (running) {
      boost::this_thread::disable_interruption di;
      MessageP message = protocol_.ReadMessage(stream_);
      protocol_.WriteMessage(stream_, Eval(*message));
    }
  }
  
  bool running_;
  std::auto_ptr<boost::thread> listen_thread_;
  std::iostream stream_;
  ProtocolP protocol_;
}

}
}
}
#endif // _TMS_COMMON_PROTOCOL__SERVER_HPP_
