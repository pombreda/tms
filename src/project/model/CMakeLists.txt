add_library (tms_project_model STATIC
  contact_person
  scheme
  incoming	
  company
  subject
  addressee
  )
target_link_libraries(tms_project_model 
  tms_contraption 
  tms_exception
  )
