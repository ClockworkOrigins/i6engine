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
# luabind cmake
################################

include_directories("include")
include_directories(${I6ENGINE_LUA_INCLUDE_DIR})

set(srcdir ./src)

set(luabindSrc
	${srcdir}/class.cpp
	${srcdir}/class_info.cpp
	${srcdir}/class_registry.cpp
	${srcdir}/class_rep.cpp
	${srcdir}/create_class.cpp
	${srcdir}/error.cpp
	${srcdir}/exception_handler.cpp
	${srcdir}/function.cpp
	${srcdir}/inheritance.cpp
	${srcdir}/link_compatibility.cpp
	${srcdir}/object_rep.cpp
	${srcdir}/open.cpp
	${srcdir}/pcall.cpp
	${srcdir}/scope.cpp
	${srcdir}/stack_content_by_name.cpp
	${srcdir}/weak_ref.cpp
	${srcdir}/wrapper_base.cpp
)

add_library(luabind STATIC ${luabindSrc})

########################
# luabind install
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS luabind
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
