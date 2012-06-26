find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(memory_contraption_test 
  memory_contraption_test.cpp
  )

target_link_libraries(memory_contraption_test 
  tms_memory_contraption 
  tms_contraption 
  ${Boost_LIBRARIES}
  )

add_test(memory_contraption_test memory_contraption_test)

