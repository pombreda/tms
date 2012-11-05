add_executable (compress compress)
add_executable (decompress decompress)
find_package(LibArchive)
find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(compress
  ${LibArchive_LIBRARIES}
  ${Boost_LIBRARIES}
  tms_archive
  )
target_link_libraries(decompress
  ${LibArchive_LIBRARIES}
  ${Boost_LIBRARIES}
  tms_archive
  )
