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
#include <boost/serialization/access.hpp>

namespace tms {
namespace common {

//------------------------------------------------------------
// Unique ids for different requests
//------------------------------------------------------------

enum RequestID {
  kRequest
};

//------------------------------------------------------------
// Class definition: Request
// Base class for all server requests
//------------------------------------------------------------

class Request {
 public:
  Request(std::string user, std::string password) 
      : user_(user),
        password_hash_(sha256(password)),
        version_(kVersion) {
  }
  
  //------------------------------------------------------------
  // Unique ID
  //------------------------------------------------------------

  virtual int RequestID() {
    return kRequest;
  }
  
  //------------------------------------------------------------
  // Getters
  //------------------------------------------------------------

  std::string user() {
    return user_;
  }
  
  std::string password_hash() {
    return password_hash_;
  }

  int version() {
    return version_;
  }
  
  Request() {} // for serialization only  
  virtual ~Request(){}
 
 private:

  //------------------------------------------------------------
  // Serialization
  //------------------------------------------------------------
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & user_;
    ar & password_hash_;
    ar & version_;
  }

  //------------------------------------------------------------
  // Data
  //------------------------------------------------------------

  std::string user_;
  std::string password_hash_;
  int version_;
};
}
}
#endif // _REQUEST_HPP_
