namespace tms {
namespace common {
namespace contraption {

template<typename T>
T Contraption::Get(FieldID field_id)
    throw(FieldException, ModelBackendException) {
  FieldTypeP ret = GetFieldValue(field_id);
  FieldTypeT<T> *ptr = dynamic_cast<FieldTypeT<T>*>(
      ret.get());
  if (ptr) {
    return ptr->data();
  } else {
    std::ostringstream msg;
    msg << "Incorrect field type in Contraption::Get - field: '"
        << GetName(field_id) << "' of type: '"
        << typeid(model_->GetField(field_id)).name()
        << "' can not be interpreted as value of type '"
        << typeid(T).name() << "'.";
    throw FieldException(msg.str());
  }
}

template<typename T>
T Contraption::Get(const std::string &field)
    throw(FieldException, ModelBackendException) {
  return Get<T>(GetID(field));
}

template<typename T>
void Contraption::Set(FieldID field_id, const T &value)
    throw(FieldException, ModelBackendException) {
  SetFieldValue(field_id, FieldTypeT<T>(value));
}

template<typename T>
void Contraption::Set(const std::string &field, const T &value)
    throw(FieldException, ModelBackendException) {
  Set(GetID(field), value);
}



}
}
}

namespace boost {
inline void intrusive_ptr_add_ref(tms::common::contraption::Contraption* contraption) {
  ++contraption->ptr_count_;
}

inline void intrusive_ptr_release(tms::common::contraption::Contraption* contraption) {
  if(!(--contraption->ptr_count_))
    delete contraption;
}
}
