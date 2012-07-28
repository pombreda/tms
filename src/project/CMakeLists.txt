add_subdirectory(model)
add_library(tms_project SHARED
  protocol
)

target_link_libraries(tms_project
  tms_protocol
)