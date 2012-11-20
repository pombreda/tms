add_executable (patcher patcher)
find_package(LibArchive)
find_package(Boost COMPONENTS filesystem system REQUIRED)
include_directories(SYSTEM ${LibArchive_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(patcher
  ${LibArchive_LIBRARIES}
  ${Boost_LIBRARIES}
  tms_archive
  tms_patcher
  )

