set(test_sources 
  request_test.cpp
  )

set(test test_protocol)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(${test} ${test_sources})

target_link_libraries(${test} tms_exception tms_protocol crypto ${Boost_LIBRARIES})

add_test(${test} ${test})