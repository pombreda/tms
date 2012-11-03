add_library(tms_archive SHARED
            archive)
find_package(LibArchive)
find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(tms_archive
  ${LibArchive_LIBRARIES}
  ${Boost_LIBRARIES}
  )
