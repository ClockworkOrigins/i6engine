MESSAGE(STATUS "Looking for SSL")

# Look for the header file.
FIND_PATH(SSL_INCLUDE_DIR NAMES ssl.h PATHS /usr/include/openssl/)

# Look for the library.
FIND_LIBRARY(SSL_LIBRARY NAMES ssl libssl)

# handle the QUIETLY and REQUIRED arguments and set SSL_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SSL DEFAULT_MSG SSL_LIBRARY SSL_INCLUDE_DIR)

# Copy the results to the output variables.
IF(SSL_FOUND)
  SET(SSL_LIBRARIES ${SSL_LIBRARY})
  SET(SSL_INCLUDE_DIRS ${SSL_INCLUDE_DIR})
  MESSAGE(STATUS "  includes  : ${SSL_INCLUDE_DIR}")
ELSE(SSL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find SSL")
ENDIF(SSL_FOUND)

MARK_AS_ADVANCED(SSL_INCLUDE_DIR SSL_LIBRARY)
