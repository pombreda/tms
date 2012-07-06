#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

//------------------------------------------------------------
// Headers
//------------------------------------------------------------

#include <string>
#include <protocol/crypto.hpp>
#include <setup.hpp>
#include <istream>
#include <ostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>

namespace tms {
namespace common {

//------------------------------------------------------------
// Unique ids for different requests
//------------------------------------------------------------

namespace RequestID {
enum RequestID {
  kRequest,
  kIncorrect
};
const size_t kAuthentication = kRequest;
}

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

  virtual size_t RequestID() {
    return RequestID::kRequest;
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
      
  Request() : user_(), password_hash_(), version_() {} // for serialization only  
  virtual ~Request(){}
 
 private:

  //------------------------------------------------------------
  // Serialization
  //------------------------------------------------------------
  friend class boost::serialization::access;

  template<class Archive>
  void save(Archive &ar, const unsigned int /*version*/) const {
    ar & user_;
    ar & password_hash_;
    ar & version_;
  }

  template<class Archive>
  void load(Archive &ar, const unsigned int /*version*/) {
    ar & user_;
    ar & password_hash_;
    ar & version_;
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()
  
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
