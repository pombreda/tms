namespace tms {
namespace common {
namespace protocol {

template<class T>
const T& Server::Get(const std::string &var) const
    throw(SessionException) {
  LOG4CPLUS_DEBUG(logger_, 
                  LOG4CPLUS_TEXT("Reading " + var));
  
  SessionMap::const_iterator it = session_.find(var);
  if (it != session_.end()) {
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
  } else {
    throw new SessionException("Trying to access variable " + var + " which "
                               + "was not set in Server::Get.");
  }
}

template<class T>
void Server::Set(const std::string &var, const T &value) 
    throw(SessionException) {
  LOG4CPLUS_DEBUG(logger_, 
                  LOG4CPLUS_TEXT("Writing " + var));

  session_[var].reset(new contraption::FieldTypeT<T>(value));
}

}
}
}
