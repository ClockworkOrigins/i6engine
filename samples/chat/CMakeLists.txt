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
# SampleChat cmake
################################

include_directories(include)
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(SampleChatClientSrc
	mainClient.cpp

	src/ChatApplication.cpp
)

set(SampleChatHostSrc
	mainHost.cpp

	src/ChatApplication.cpp
)

add_executable(SampleChatClient ${GUI_TYPE} ${SampleChatClientSrc})
set_target_properties(SampleChatClient PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleChatClient i6engine-modules)

add_executable(SampleChatHost ${GUI_TYPE} ${SampleChatHostSrc})
set_target_properties(SampleChatHost PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(SampleChatHost i6engine-modules)

IF(WIN32)
	add_custom_command(TARGET SampleChatClient POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleChatClient.exe ${CMAKE_BINARY_DIR}/bin)
	add_custom_command(TARGET SampleChatHost POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/SampleChatHost.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS SampleChatClient
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)

install(TARGETS SampleChatHost
		DESTINATION ${CMAKE_INSTALL_PREFIX}/samples
	)
