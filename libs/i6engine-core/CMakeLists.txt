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
# i6gengine-core cmake
################################

include_directories("include")
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(srcdir ./src)
set(utildir /util)
set(configdir ${CMAKE_SOURCE_DIR}/config)

# Core source files
set(i6engineCoreSrc
	${i6engineCoreSrc}
	${srcdir}/EngineCoreController.cpp

	${srcdir}/clock/Timer.cpp

	${srcdir}/messaging/Message.cpp
	${srcdir}/messaging/IPKey.cpp
	${srcdir}/messaging/MessageSubscriber.cpp
	${srcdir}/messaging/MessagingController.cpp

	${srcdir}/subsystem/ModuleController.cpp
	${srcdir}/subsystem/SubSystemController.cpp
)


# SOURCE_GROUP required for file grouping in Visual Studio
# SOURCE_GROUP does not yet work with Visual Studio 2010
# -> Confirmed bug in CMAKE Mantis *grml*
source_group(core\\clock /clock/*)
source_group(core\\messaging /messaging/*)
source_group(core\\subsystem /subsystem/*)
source_group(core /*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-core SHARED ${i6engineCoreSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-core STATIC ${i6engineCoreSrc})
ENDIF(ISIXE_BUILD_SHARED)

set_target_properties(i6engine-core PROPERTIES LINKER_LANGUAGE CXX)
# to find the dependency libs during runtime
set_target_properties(i6engine-core PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

target_link_libraries(i6engine-core i6engine-utils)

IF(ISIXE_WITH_PROFILING)
	target_link_libraries(i6engine-core ${GPerfTools_LIBRARIES})
ENDIF(ISIXE_WITH_PROFILING)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-core POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-core.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

########################
# i6engine-core install
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-core
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
