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

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

SET(testSrc
	general/test_BoostVSSTL.cpp
	
	i6engine-utils/test_RangedMap.cpp
	
	i6engine-core/test_Message.cpp
	i6engine-core/test_Module.cpp
	
	i6engine-modules/test_GameMessage.cpp
	i6engine-modules/test_GameObject.cpp
)

add_executable(MeasurementTester ${testSrc})

SET_TARGET_PROPERTIES(MeasurementTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(MeasurementTester i6engine-modules ${GTEST_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET MeasurementTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/MeasurementTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)
	
IF(UNIX)
	target_link_libraries(MeasurementTester pthread)
ENDIF(UNIX)
