add_library(tms_string SHARED
            string)
find_package(ICONV)
include_directories(SYSTEM ${ICONV_INCLUDE_DIR})
target_link_libraries(tms_string 
  tms_exception
  ${ICONV_LIBRARIES}
  )
