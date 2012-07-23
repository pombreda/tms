#ifndef _TMS_SRC_COMMON_CLIENT__PRINTER_HPP_
#define _TMS_SRC_COMMON_CLIENT__PRINTER_HPP_

#include <boost/lexical_cast.hpp>
#include <contraption/field_type.hpp>
#include <cstdio>

namespace tms {
namespace common {
namespace widget {

class Printer {
 public:
  virtual std::string ToString(contraption::FieldType &t) = 0;
  virtual ~Printer() {};
};

template<class T>
class PrinterT : public Printer {
 public:
  std::string ToString(contraption::FieldType &t) {
    contraption::FieldTypeT<T> *ptr = dynamic_cast<contraption::FieldTypeT<T>*>(&t);
//    if (!ptr) {
//      throw(...);
//    }
    return boost::lexical_cast<std::string>(ptr->data());
  }
};


}
}
}

#endif
