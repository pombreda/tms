using namespace std;
#include <boost/mpl/assert.hpp>
#include <boost/mpl/aux_/config/static_constant.hpp>
namespace tms {
namespace common {
namespace contraption {

template<class T>
FieldTypeID GetFieldTypeID();

//------------------------------------------------------------
// Macro fun
//------------------------------------------------------------

#undef TMS_FIELD_TYPE_INIT_LIB
#undef INIT_FIELD_TYPE

#define TMS_FIELD_TYPE_CALC_SLIBID(libid)                               \
  /**Macro body**/                                                      \
  (                                                                     \
      static_cast<FieldTypeID>(libid) << 8                              \
  )                                                                     \
  /**Macro end**/

#define TMS_FIELD_TYPE_CALC_ID(libid, id)                               \
  /**Macro body**/                                                      \
  (                                                                     \
    TMS_FIELD_TYPE_CALC_SLIBID(libid) |                                 \
    (                                                                   \
        static_cast<FieldTypeID>(id)                                    \
        &                                                               \
        (                                                               \
            static_cast<FieldTypeID>(-1) >> 8                           \
        )                                                               \
    )                                                                   \
  )                                                                     \
  /**Macro end**/

template<int id>
class FieldType_lib {
};

#define TMS_FIELD_TYPE_INIT_LIB(libid)                                  \
  /**Macro body**/                                                      \
  namespace tms {                                                       \
  namespace common {                                                    \
  namespace contraption {                                               \
  template<>                                                            \
  class FieldType_lib<libid> {                                          \
   public:                                                              \
   static const FieldTypeID v = TMS_FIELD_TYPE_CALC_SLIBID(libid);      \
  };                                                                    \
  }                                                                     \
  }                                                                     \
  }                                                                     \
  /**Macro end**/

template<int libid, int id>
class FieldType_id {
};

#define INIT_FIELD_TYPE(type, libid, id)                                \
  /**Macro body**/                                                      \
  namespace tms {                                                       \
  namespace common {                                                    \
  namespace contraption {                                               \
  template<>                                                            \
  class FieldType_id<libid, id> {                                       \
   public:                                                              \
   static const FieldTypeID v =                                         \
       TMS_FIELD_TYPE_CALC_ID(                                          \
           FieldType_lib<libid>::v,                                     \
           id);                                                         \
  };                                                                    \
                                                                        \
  template<>                                                            \
  FieldTypeID GetFieldTypeID<type>() {                                  \
    return FieldType_id<libid, id>::v;                                  \
  }                                                                     \
  }                                                                     \
  }                                                                     \
  }                                                                     \
  /**Macro end**/

template<class T>
struct field_type_not_inited
{
  BOOST_STATIC_CONSTANT(bool, value = false);
  typedef field_type_not_inited<T> type;
  typedef bool value_type;
};

template<class T>
FieldTypeID GetFieldTypeID() {
  BOOST_MPL_ASSERT(( field_type_not_inited<T> ));
  return 0;
}

//------------------------------------------------------------
// FieldTypeT<T> realization
//------------------------------------------------------------

template<class T>
FieldTypeT<T>::FieldTypeT() : id_(GetFieldTypeID<T>()) {}

template<class T>
void* FieldTypeT<T>::Duplicate(const void *object) const {
  return static_cast<void *>(new T(*static_cast<const T *>(object)));
}

template<class T>
void FieldTypeT<T>::Free(void *object) const {
  delete static_cast<T *>(object);
}

template<class T>
void* FieldTypeT<T>::New() const {
  return static_cast<void *>(new T());
}

}
}
}
