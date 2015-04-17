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
# SampleTerrain cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(SampleTerrainSrc
	main.cpp

	src/TerrainApplication.cpp
)

add_executable(SampleTerrain ${SampleTerrainSrc})
set_target_properties(SampleTerrain PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleTerrain i6engine-modules)

IF(WIN32)
	add_custom_command(TARGET SampleTerrain POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleTerrain.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleTerrain
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)