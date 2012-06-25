#ifndef _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
#define _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
//------------------------------------------------------------
// Headers
//------------------------------------------------------------
#include <cstdlib>
namespace tms {
namespace common {
namespace contraption {

typedef size_t FieldTypeID;
// Specialize this function for your type
template<class T>
FieldTypeID GetFieldTypeID();

//------------------------------------------------------------
// FieldType interface.
//------------------------------------------------------------

class FieldType {
 public:
  virtual void *Duplicate(const void *object) = 0 const;
  virtual void Free(void *object) = 0 const;
  virtual void *New() = 0 const;
  virtual FieldTypeID id() = 0 const;
  virtual ~FieldType(){}
};
                                               
//------------------------------------------------------------
// FieldTypeT<T> class. Wrapper for a c++ type.
// Correct copy construcor required
// Default constructor required
//------------------------------------------------------------
template<class T>
class FieldTypeT : FieldType {
 public:
  FieldTypeT();
  // Static casts int this functions. A bit risky.
  virtual void *Duplicate(const void *object) const;
  virtual void Free(void *object) const;
  virtual void *New() const;
  virtual FieldTypeID id() const {return id_;}
  virtual ~FieldTypeT(){}
 private:
  const FieldTypeID id_;
};

// Define this macro once for your library
// Try to avoid id collision between difference libraries in
// one project
// libid - 24bit
#define TMS_FIELD_TYPE_INIT_LIB(libid)                                  

// Define this macro for your type
// Try to avoid id collision in one library
// Library with libid must be inited
// id - 8bit
#define INIT_FIELD_TYPE(type, libid, id)                                

}
}
}
#include "field_type_impl.cpp"
#endif // _TMS_COMMON_CONTRAPTION__FIELD_TYPE_HPP_
