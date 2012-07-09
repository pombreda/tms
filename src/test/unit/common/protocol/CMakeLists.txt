find_package(Boost COMPONENTS unit_test_framework serialization REQUIRED)

add_executable(crypto_test 
  crypto_test.cpp
)
target_link_libraries(crypto_test tms_exception tms_protocol ${Boost_LIBRARIES})
add_test(crypto_test crypto_test)