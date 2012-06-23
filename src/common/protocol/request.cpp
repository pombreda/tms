#include "request.hpp"
#include <protocol/serialization_exception.hpp>
#include <sstream>
#include <iterator>
#include <exception>

using namespace tms::common;
using namespace std;

string Request::Mask(const string &str) {
  ostringstream ret(ostringstream::binary);
  for (size_t pos = 0; pos < str.size(); ++pos) {
    switch (str[pos]) {
      case '\\':
        ret << "\\\\";
        break;
      case '\0':
        ret << '\\' << '\0';
        break;      
      default:
        ret << str[pos];
    }
  }
  return ret.str();
}

string Request::Unmask(const string &str) {
  ostringstream ret(ostringstream::binary);
  for (size_t pos = 0; pos < str.size(); ++pos) {
    if (str[pos] == '\\') {
      if (pos == str.size() - 1) {
        ostringstream msg;
        msg << "Incorrect SerializationID in Request::Deserialize.\n"
            << "String ends with a single \\." << endl;
        throw SerializationException(msg.str());
      } else {
        if (str[pos + 1] != '\0' && str[pos + 1] != '\\') {
          ostringstream msg;
          msg << "Incorrect SerializationID in Request::Deserialize.\n"
              << "Unknown escape sequence \"\\[" << (int)str[pos + 1] 
              << "]\"." << endl;
          throw SerializationException(msg.str());
        }
        ++pos;
      }
    } 
    ret << str[pos];
  }
  return ret.str();
}

void Request::Serialize(ostream &sout) {
  try {
    sout << (int) kRequest << " ";
    Save(sout);
  } catch (exception e) {
    throw new SerializationException(&e, "Exception in Request::Serialize");
  }
}

string Request::ReadPart(istream &sin) {
  ostringstream ret;
  for (istream_iterator<char> it(sin); *it != '\0'; ++it) {
    if (*it == '\\') {
      ++it;
    }      
    ret << *it;
  }
  return ret.str();
}

void Request::Save(std::ostream &sout) {
    sout << Mask(user_) << '\0'
         << Mask(password_hash_) << '\0' 
         << version_ << '\0';
}

void Request::Load(istream &sin) {
  user_ = ReadPart(sin);
  password_hash_ = ReadPart(sin);
  istringstream conv(ReadPart(sin));
  conv >> version_;
}


Request* Request::Deserialize(istream &sin) {
  try {
    int serialization_id;
    sin >> serialization_id;
    Request *ret;
    switch (serialization_id) {
      case kRequest:        
        ret = new Request();
        ret->Load(sin);
        return ret;
      default:
        ostringstream msg;
        msg << "Incorrect SerializationID in Request::Deserialize.\n"
            << "SerializationID recieved: " << serialization_id << "." << endl;
        throw new SerializationException(msg.str());
    }
  } catch (exception e) {
    throw new SerializationException(&e, "Exception in Request::Deserialize");
  }
}
