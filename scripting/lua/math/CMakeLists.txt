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
# LuaMathExport cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_LUA_INCLUDE_DIR})
include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})

set(srcdir .)

set(luaMathSrc
	${srcdir}/LuaMathExport.cpp
)

add_library(ScriptingMathLua SHARED ${luaMathSrc})

target_link_libraries(ScriptingMathLua i6engine-math lua luabind)
# to find the dependency libs during runtime
set_target_properties(ScriptingMathLua PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

IF(WIN32)
	add_custom_command(TARGET ScriptingMathLua POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ScriptingMathLua.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

########################
# LuaMathExport install
########################

install(TARGETS ScriptingMathLua
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
