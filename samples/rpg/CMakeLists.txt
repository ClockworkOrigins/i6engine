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
# SampleRPG cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_RPG_INCLUDE_DIR})

set(SampleRPGSrc
	main.cpp

	src/RPGApplication.cpp
)

add_executable(SampleRPG ${SampleRPGSrc})
set_target_properties(SampleRPG PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleRPG i6engine-rpg)

IF(WIN32)
	add_custom_command(TARGET SampleRPG POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleRPG.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleRPG
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)
