##
# Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##

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
