//------------------------------------------------------------
// Implementation of template functions of Contraption class.
//------------------------------------------------------------
#include <sstream>
template<class T>
void Contraption::GetField(size_t field_id, T &out) const
      throw(FieldExcepton, ModelBackendException) {
  if (FieldType(field_id)::ID != FieldType<T>::ID) {
    ostringstream msg;
    msg << "Incompatible field types in GetField: "
        << FieldType(field_id)::ID << " != "
        << FieldType<T>::ID;
    throw FieldExcepton(msg.str());
  }
  if (!values_[field_id]) {
    model_->GetField(field_id, id)
  }
  out = *(T *)(values_[field_id]);
}

template<class T>
void Contraption::GetField(const std::string &field, int &out) const
    throw(FieldExcepton, ModelBackendException) {
  GetField(GetFieldID(field), out);
}

template<class T>
void Contraption::SetField(size_t field_id, const T &value)
    throw(FieldExcepton) {
  if (GetFieldType(field_id).ID() != FieldType<T>::ID) {
    ostringstream msg;
    msg << "Incompatible field types in SetField: "
        << FieldType(field_id)::ID << " != "
        << FieldType<T>::ID;
  }
  if (values_[field_id]) {
    *(T *)(values_[field_id]) = value;
  } else {
    values_[field_id] = new T(value);
  }
}

template<class T>
void SetField(const std::string &field, const T &value)
    throw(FieldExcepton) {
  SetField(GetFieldID(field), value);
}
