add_library(tms_validators SHARED
  string_validator
  password_validator
  bool_validator
  hide_if_validator
  has_one_validator
  has_many_validator
)

find_package(wxWidgets COMPONENTS base core REQUIRED)
include( "${wxWidgets_USE_FILE}" )
find_package(Boost COMPONENTS signals REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${wxWidget_INCLUDE_DIR})


target_link_libraries(tms_validators
  tms_protocol
  tms_contraption
  ${wxWidgets_LIBRARIES}
  )
