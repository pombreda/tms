set(test_sources 
  gui_exception_test.cpp
  )
set(test test_gui_exception)

add_executable(${test} ${test_sources})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include( "${Boost_USE_FILE}" )

target_link_libraries(${test} tms_exception ${Boost_LIBRARIES})

add_test(${test} ${test})