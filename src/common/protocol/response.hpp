#ifndef _RESPONSE_HPP_
#define _RESPONSE_HPP_

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
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/version.hpp>

namespace tms {
namespace common {

//------------------------------------------------------------
// Unique ids for different requests
//------------------------------------------------------------

namespace ResponseID {
enum ResponseID {
  kResponse,
  kIncorrect
};
}

namespace ResponseStatus {
enum ResponseStatus {
  kOk,
  kAuthenticationError,
  kIncorrectVersion,
  kServerError,
  kIncorrect
};
}

//------------------------------------------------------------
// Class definition: Request
// Base class for all server requests
//------------------------------------------------------------

class Response {
 public:
  Response(size_t status) 
      : status_(status) {
  }
  
  //------------------------------------------------------------
  // Unique ID
  //------------------------------------------------------------

  virtual size_t ResponseID() {
    return ResponseID::kResponse;
  }
  
  //------------------------------------------------------------
  // Getters
  //------------------------------------------------------------

  size_t status() {
    return status_;
  }

  Response() : status_(ResponseID::kIncorrect) {} // for serialization only  
  virtual ~Response(){}
 
 private:

  //------------------------------------------------------------
  // Serialization
  //------------------------------------------------------------
  friend class boost::serialization::access;

  template<class Archive>
  void save(Archive &ar, const unsigned int /*version*/) const {
    ar & status_;
  }

  template<class Archive>
  void load(Archive &ar, const unsigned int /*version*/) {
    ar & status_;
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

  //------------------------------------------------------------
  // Data
  //------------------------------------------------------------
  size_t status_;
};
}
}

#endif // _RESPONSE_HPP_
