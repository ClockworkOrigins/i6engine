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

################################
# i6gengine-core testing cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})

SET(testSrc
	clock/test_Scheduler.cpp

	messaging/test_IPKey.cpp
	messaging/test_Message.cpp
	messaging/test_MessagingController.cpp

	subsystem/test_Module.cpp
)
add_executable(CoreTester ${testSrc})

SET_TARGET_PROPERTIES(CoreTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(CoreTester i6engine-core ${GTEST_LIBRARIES})

IF(NOT WIN32)
	target_link_libraries(CoreTester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET CoreTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/CoreTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

ADD_TEST(CoreTester CoreTester)
