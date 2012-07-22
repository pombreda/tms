#ifndef _TMS_COMMON_PROTOCOL__LOGIN_SERVER_HPP_
#define _TMS_COMMON_PROTOCOL__LOGIN_SERVER_HPP_
namespace tms {
namespace common {
namespace protocol {

class LoginServer : public Server {
 public: 
  LoginServer(ServerP server);
  virtual void Listen()
      thread
 private:
  ServerP server_;
};

}
}
}
#endif // _TMS_COMMON_PROTOCOL__LOGIN_SERVER_HPP_
