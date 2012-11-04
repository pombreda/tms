find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Log4cplus)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${LOG4CPLUS_INCLUDE_DIR})
add_subdirectory(model)
add_library(tms_project SHARED
  protocol
)

target_link_libraries(tms_project
  tms_protocol
)