#ifndef _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
#define _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <string>

namespace tms {
namespace common {
namespace contraption {
class Record {
 public:
  virtual ~Record() {}
};

template<typename T>
class RecordT : public Record {
 public:
  RecordT(const std::string &field, T *data) :
      field(field),
      data(data) {}
  const std::string &field;
  T *data;
 private:
  RecordT(const RecordT<T> &other);
  RecordT<T>& operator=(const RecordT<T> &other);
};
}
}
}
#endif // _TMS_COMMON_CONTRAPTION__RECORD_FWD_HPP_
