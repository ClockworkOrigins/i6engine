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
# i6gengine-utils testing cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

SET(testSrc
	test_AutoUpdater.cpp
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
