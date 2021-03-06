MESSAGE(STATUS "Looking for Xrandr")

# Look for the header file.
FIND_PATH(XRANDR_INCLUDE_DIR NAMES Xrandr.h PATHS /usr/include/X11/extensions/)

# Look for the library.
FIND_LIBRARY(XRANDR_LIBRARY NAMES Xrandr libXrandr)

# handle the QUIETLY and REQUIRED arguments and set XRANDR_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XRANDR DEFAULT_MSG XRANDR_LIBRARY XRANDR_INCLUDE_DIR)

# Copy the results to the output variables.
IF(XRANDR_FOUND)
  SET(XRANDR_LIBRARIES ${XRANDR_LIBRARY})
  SET(XRANDR_INCLUDE_DIRS ${XRANDR_INCLUDE_DIR})
  MESSAGE(STATUS "  includes  : ${XRANDR_INCLUDE_DIR}")
ELSE(XRANDR_FOUND)
  MESSAGE(FATAL_ERROR "Could not find XRANDR")
ENDIF(XRANDR_FOUND)

MARK_AS_ADVANCED(XRANDR_INCLUDE_DIR XRANDR_LIBRARY)
