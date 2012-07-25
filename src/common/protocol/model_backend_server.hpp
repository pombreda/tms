#ifndef _TMS_COMMON_PROTOCOL__MODEL_BACKEND_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__MODEL_BACKEND_SERVER_HPP_
namespace tms {
namespace common {
namespace protocol {

class ModelBackendServer : public LoginServer {
 public: 
  ModelBackendServer(ServerP server);
  virtual void Listen()
      thread
 private:
  ServerP server_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__MODEL_BACKEND_SERVER_HPP_
