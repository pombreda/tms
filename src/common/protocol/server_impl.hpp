#include <string/string.hpp>
namespace tms {
namespace common {
namespace protocol {

template<class T>
T& Server::Get(const std::string &var)
    throw(SessionException) {
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String("Reading " + var));
  
  SessionMap::const_iterator it = session_.find(var);
  if (it == session_.end()) {
    session_[var].reset(new contraption::FieldTypeT<T>());
    it = session_.find(var);
  } 
  contraption::FieldTypeT<T> *value 
      = dynamic_cast<contraption::FieldTypeT<T>*>(&*it->second);
  if (value) {
    return value->data();
  } else {
    throw new SessionException("Incorrect variable type for " 
                               + var + " in Server::Get." + " Expected: " 
                               + rtti::TypeID<T>().name() + ", but was: " 
                               + rtti::TypeID(*it->second).name());
  }
}

template<class T>
void Server::Set(const std::string &var, const T &value) 
    throw(SessionException) {
  LOG4CPLUS_DEBUG(logger_, 
                  string::WStringFromUTF8String("Writing " + var));
  
  session_[var].reset(new contraption::FieldTypeT<T>(value));
}

}
}
}
