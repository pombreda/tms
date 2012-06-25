template<class T>
void Contraption::GetField(size_t field_id, T &out) const {
  if (FieldType(field_id)::ID != FieldType<T>::ID) {
    //oops
  }
  if (!values_[field_id]) {
    model_->GetField(this, field_id)
  }
  out = *(T *)(values_[field_id]);
}

template<class T>
void Contraption::GetField(const std::string &field, int &out) const {
  GetField(GetFieldID(field), out);
}

template<class T>
void Contraption::SetField(size_t field_id, const T &value) {
  if (GetFieldType(field_id).ID() != FieldType<T>::ID) {
    //oops
  }
  if (values_[field_id]) {
    *(T *)(values_[field_id]) = value;
  } else {
    values_[field_id] = new T(value);
  }
}

template<class T>
void SetField(const std::string &field, const T &value) {
  SetField(GetFieldID(field), value);
}
