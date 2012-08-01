find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
add_subdirectory(model)
add_library(tms_project SHARED
  protocol
)

target_link_libraries(tms_project
  tms_protocol
)