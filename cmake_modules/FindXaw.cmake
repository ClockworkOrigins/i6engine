# +-----------------------------------------------------------------------------+
# | $Id:: FindXaw.cmake 2818 2009-07-15 19:02:31Z baehren                     $ |
# +-----------------------------------------------------------------------------+
# |   Copyright (C) 2007                                                        |
# |   Lars B"ahren (bahren@astron.nl)                                           |
# |                                                                             |
# |   This program is free software; you can redistribute it and/or modify      |
# |   it under the terms of the GNU General Public License as published by      |
# |   the Free Software Foundation; either version 2 of the License, or         |
# |   (at your option) any later version.                                       |
# |                                                                             |
# |   This program is distributed in the hope that it will be useful,           |
# |   but WITHOUT ANY WARRANTY; without even the implied warranty of            |
# |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
# |   GNU General Public License for more details.                              |
# |                                                                             |
# |   You should have received a copy of the GNU General Public License         |
# |   along with this program; if not, write to the                             |
# |   Free Software Foundation, Inc.,                                           |
# |   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 |
# +-----------------------------------------------------------------------------+

# - Check for the presence of XAW
#
# The following variables are set when XAW is found:
#  HAVE_XAW       = Set to true, if all components of XAW have been found.
#  XAW_INCLUDES   = Include path for the header files of XAW
#  XAW_LIBRARIES  = Link these to use XAW
#  XAW_LFGLAS     = Linker flags (optional)

MESSAGE(STATUS "Looking for XAW")

## -----------------------------------------------------------------------------
## Search locations

## -----------------------------------------------------------------------------
## Check for the header files

  FIND_PATH( XAW_INCLUDES NAMES XawImP.h XawInit.h
    PATHS  /usr/include/X11/Xaw/)

## -----------------------------------------------------------------------------
## Check for the libraries

FIND_LIBRARY(XAW_LIBRARIES_Xaw
  NAMES Xaw
  PATHS
  /usr/lib/
  /usr/lib64/
)

FIND_LIBRARY(XAW_LIBRARIES_Xaw7
  NAMES Xaw7
  PATHS
  /usr/lib/
  /usr/lib64/
)


SET( XAW_LIBRARIES
  ${XAW_LIBRARIES_Xaw}
  ${XAW_LIBRARIES_Xaw7}
)


## -----------------------------------------------------------------------------
## Actions taken when all components have been found

if (XAW_INCLUDES AND XAW_LIBRARIES)
  set (HAVE_XAW TRUE)
else (XAW_INCLUDES AND XAW_LIBRARIES)
  set (HAVE_XAW FALSE)
  if (NOT XAW_FIND_QUIETLY)
    if (NOT XAW_INCLUDES)
      message (STATUS "Unable to find XAW header files!")
    endif (NOT XAW_INCLUDES)
    if (NOT XAW_LIBRARIES)
      message (STATUS "Unable to find XAW library files!")
    endif (NOT XAW_LIBRARIES)
  endif (NOT XAW_FIND_QUIETLY)
endif (XAW_INCLUDES AND XAW_LIBRARIES)

if (HAVE_XAW)
  if (NOT XAW_FIND_QUIETLY)
    MESSAGE(STATUS "  libraries : ${XAW_LIBRARIES}")
    MESSAGE(STATUS "  includes  : ${XAW_INCLUDES}")
  endif (NOT XAW_FIND_QUIETLY)
else (HAVE_XAW)
  if (XAW_FIND_REQUIRED)
    message (FATAL_ERROR "Could not find XAW")
  endif (XAW_FIND_REQUIRED)
endif (HAVE_XAW)

## -----------------------------------------------------------------------------
## Mark advanced variables

mark_as_advanced (
  XAW_LIBRARIES_Xaw
  XAW_LIBRARIES_Xaw7
  XAW_INCLUDES
  XAW_LIBRARIES
  XAW_LIBRARIES_Xaw
  XAW_LIBRARIES_Xaw7
  )

