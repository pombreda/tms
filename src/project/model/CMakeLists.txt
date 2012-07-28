add_library (tms_project_model SHARED
  contact_person
  )
target_link_libraries(tms_project_model 
  tms_contraption 
  tms_exception
  )