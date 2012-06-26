#ifndef _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
#define _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_

class ModelBackend {
  virtual FieldType GetField(field_id, contraption) = 0;
  virtual Save(contraption) = 0;
};

#endif // _TMS_COMMON_CONTRAPTION__MODEL_BACKEND_HPP_
