find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(field_type_test field_type_test.cpp)
target_link_libraries(field_type_test ${Boost_LIBRARIES})
add_test(field_type_test field_type_test)
