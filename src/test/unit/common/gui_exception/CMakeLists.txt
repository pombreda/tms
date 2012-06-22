set(test_sources 
  gui_exception_test.cpp
  )
set(test test_gui_exception)

add_executable(${test} ${test_sources})

set(dlls tms_exception boost_unit_test)
target_link_libraries(${test} ${dlls})


foreach(dll ${dlls})
  get_target_property(path ${dll} LOCATION)
  add_custom_command(
   TARGET ${test}
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${path} ${CMAKE_CURRENT_BINARY_DIR}
   )
endforeach()

foreach(path ${stdlibs})
  add_custom_command(
   TARGET ${test}
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${path} ${CMAKE_CURRENT_BINARY_DIR}
   )
endforeach()

add_test(${test} ${test})