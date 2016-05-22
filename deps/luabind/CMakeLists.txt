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
