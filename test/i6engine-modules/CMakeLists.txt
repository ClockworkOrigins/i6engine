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
# i6gengine-modules testing cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

SET(testSrc
	api/test_GameMessage.cpp
	
	api/components/test_Component.cpp
	api/components/test_ComponentCreateException.cpp
	api/components/test_ComponentSynchronize.cpp
	api/components/test_MoverComponent.cpp
	
	api/manager/test_IDManager.cpp
	api/manager/test_TextManager.cpp

	api/objects/test_GameObject.cpp
)
add_executable(ModulesTester ${testSrc})

SET_TARGET_PROPERTIES(ModulesTester PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(ModulesTester ${GTEST_LIBRARIES} i6engine-modules)

IF(NOT WIN32)
	target_link_libraries(ModulesTester pthread)
ENDIF(NOT WIN32)

IF(WIN32)
	add_custom_command(TARGET ModulesTester POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ModulesTester.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

ADD_TEST(ModulesTester ModulesTester)
