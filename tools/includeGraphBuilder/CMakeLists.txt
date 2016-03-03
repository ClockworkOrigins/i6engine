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
# includeGraphBuilder cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(includeGraphBuilderSrc
	main.cpp
)

add_executable(includeGraphBuilder ${includeGraphBuilderSrc})
set_target_properties(includeGraphBuilder PROPERTIES LINKER_LANGUAGE CXX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.bat.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.bat @ONLY)

IF(UNIX)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.sh.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.sh @ONLY)
ELSEIF(WIN32)
	add_custom_command(TARGET includeGraphBuilder POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/includeGraphBuilder.exe ${CMAKE_BINARY_DIR}/bin)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/includeGraphBuilder.bat.in ${CMAKE_BINARY_DIR}/bin/includeGraphBuilder.bat @ONLY)
ENDIF(UNIX)

install(TARGETS includeGraphBuilder
		DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
	)
