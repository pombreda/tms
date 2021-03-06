# Locate Log4cplus library
# This module defines
#  LOG4CPLUS_FOUND, if false, do not try to link to Log4cplus
#  LOG4CPLUS_LIBRARIES
#  LOG4CPLUS_INCLUDE_DIR, where to find log4cplus.hpp

FIND_PATH(LOG4CPLUS_INCLUDE_DIR log4cplus/logger.h
  HINTS
  $ENV{LOG4CPLUS_DIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(LOG4CPLUS_LIBRARIES
  NAMES LOG4CPLUS log4cplus log4cplusU
  HINTS
  $ENV{LOG4CPLUS_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)


INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOG4CPLUS_FOUND to TRUE if 
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Log4cplus DEFAULT_MSG LOG4CPLUS_LIBRARIES LOG4CPLUS_INCLUDE_DIR)



