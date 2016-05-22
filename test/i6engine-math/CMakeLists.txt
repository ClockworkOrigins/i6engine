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
