#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <string>
#include <protocol/crypto.hpp>
#include <protocol/serialization_id.hpp>
#include <setup.hpp>
#include <istream>
#include <ostream>

namespace tms {
namespace common {

//------------------------------------------------------------
// Class definition: Request
// Base class for all server requests
// Serialization format: fields splited by "\\\0" and 
// class ended by "\\\0""
//------------------------------------------------------------

class Request {
 public:
  Request(std::string user, std::string password) 
      : user_(user),
        password_hash_(sha256(password)),
        version_(kVersion) {
  }
  
  void Serialize(std::ostream &sout);  
  static Request* Deserialize(std::istream &sin);

  std::string user() {
    return user_;
  }
  std::string password_hash() {
    return password_hash_;
  }
  int version() {
    return version_;
  }

  virtual ~Request(){}

  virtual int SerializationID() {
    return kRequest;
  }
  static std::string Mask(const std::string &str);
 protected:
  static std::string Unmask(const std::string &str);
  virtual std::string ReadPart(std::istream &sin);
  // id has bean already read from the stream. 
  // To be called from Deserialize().  
  virtual void Load(std::istream &sin); 
  // id has bean already read from the stream. 
  // To be called from Deserialize().  
  virtual void Save(std::ostream &sout); 

 private:
  // To be called from Deserialize().
  Request() {}
  ////////////
  std::string user_;
  std::string password_hash_;
  int version_;
};
}
}
#endif // _REQUEST_HPP_
