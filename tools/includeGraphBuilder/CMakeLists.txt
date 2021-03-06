# i6engine
# Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
#
# This file is part of i6engine; i6engine is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

################################
# includeGraphBuilder cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(includeGraphBuilderSrc
	main.cpp
)

IF(WIN32)
   set(WINDOWS_RC_FILES
         icon.rc
         )
ENDIF(WIN32)

add_executable(includeGraphBuilder ${includeGraphBuilderSrc} ${WINDOWS_RC_FILES})
set_target_properties(includeGraphBuilder PROPERTIES LINKER_LANGUAGE CXX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.bat.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.bat @ONLY)

IF(UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.sh.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.sh @ONLY)
ELSEIF(WIN32)
	add_custom_command(TARGET includeGraphBuilder POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/includeGraphBuilder.exe ${CMAKE_BINARY_DIR}/bin)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.bat.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.bat @ONLY)
ENDIF(UNIX)

install(TARGETS includeGraphBuilder
		DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
	)
