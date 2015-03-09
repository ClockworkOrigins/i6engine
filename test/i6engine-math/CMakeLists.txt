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
# i6gengine-math testing cmake
################################

include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

SET(testSrc
	test_i6eMath.cpp
	test_i6eMatrix.cpp
	test_i6eQuaternion.cpp
	test_i6eVector.cpp
	test_i6eVector2.cpp
	test_i6eVector4.cpp
)
add_executable(MathTester ${testSrc})

SET_TARGET_PROPERTIES(MathTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(MathTester i6engine-math ${GTEST_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET MathTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/MathTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)
	
IF(NOT WIN32)
	target_link_libraries(MathTester pthread)
ENDIF(NOT WIN32)

ADD_TEST(MathTester MathTester)
