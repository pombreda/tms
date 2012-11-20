add_library(tms_patcher SHARED
            patcher)
find_package(LibArchive)
find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
find_package(Log4cplus REQUIRED)
include_directories( "${LOG4CPLUS_INCLUDE_DIR}" )
target_link_libraries(tms_patcher
  tms_archive
  tms_string
  ${LibArchive_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LOG4CPLUS_LIBRARIES}
  )
