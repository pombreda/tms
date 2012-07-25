#include <google/protobuf/stubs/common.h>

struct RemoveProtobuf {
  ~RemoveProtobuf() {
    google::protobuf::ShutdownProtobufLibrary();
  }
} dummy;


