find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(request_test request_test.cpp)
target_link_libraries(request_test tms_exception tms_protocol ${Boost_LIBRARIES})
add_test(request_test request_test)

add_executable(crypto_test crypto_test.cpp)
target_link_libraries(crypto_test tms_exception tms_protocol ${Boost_LIBRARIES})
add_test(crypto_test crypto_test)