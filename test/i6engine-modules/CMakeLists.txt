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
# i6gengine-modules testing cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

SET(testSrc
	api/test_GameMessage.cpp
	
	api/components/test_ComponentCreateException.cpp
	api/components/test_ComponentSynchronize.cpp
	api/components/test_MoverComponent.cpp
	
	api/facades/test_ObjectFacade.cpp
	
	api/manager/test_IDManager.cpp
	api/manager/test_TextManager.cpp

	api/objects/test_GameObject.cpp

	modules/physics/test_CollisionShape.cpp
)
add_executable(ModulesTester ${testSrc})

SET_TARGET_PROPERTIES(ModulesTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(ModulesTester ${GTEST_LIBRARIES} i6engine-modules SampleCommon)

IF(NOT WIN32)
	target_link_libraries(ModulesTester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET ModulesTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ModulesTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

ADD_TEST(ModulesTester ModulesTester)
