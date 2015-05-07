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
# i6gengine-utils testing cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

SET(testSrc
	test_AutoUpdater.cpp
	test_BoostVSSTL.cpp
	test_Clock.cpp
	test_DoubleBufferQueue.cpp
	test_Future.cpp
	test_i6eRandom.cpp
	test_i6eString.cpp
	test_RangedMap.cpp
	test_RealTimeClock.cpp
	test_Semaphore.cpp
	test_sequence_map.cpp
	test_sharedPtr.cpp
	test_Singleton.cpp
)
add_executable(UtilsTester ${testSrc})

SET_TARGET_PROPERTIES(UtilsTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(UtilsTester i6engine-utils ${GTEST_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET UtilsTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/UtilsTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)
	
IF(NOT WIN32)
	target_link_libraries(UtilsTester pthread)
ENDIF(NOT WIN32)

ADD_TEST(UtilsTester UtilsTester)
