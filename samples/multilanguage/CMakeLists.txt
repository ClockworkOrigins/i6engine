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
# SampleMultilanguage cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(SampleMultilanguageSrc
	main.cpp

	src/MultilanguageApplication.cpp
)

if(WIN32)
   set(WINDOWS_RC_FILES
         resources/icon.rc
         )
    source_group("Resources" FILES ${WINDOWS_RC_FILES})
endif()

add_executable(SampleMultilanguage ${GUI_TYPE} ${SampleMultilanguageSrc} ${WINDOWS_RC_FILES})
set_target_properties(SampleMultilanguage PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleMultilanguage i6engine-modules)

IF(UNIX)
	target_link_libraries(SampleMultilanguage pthread)
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET SampleMultilanguage POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleMultilanguage.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleMultilanguage
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)
