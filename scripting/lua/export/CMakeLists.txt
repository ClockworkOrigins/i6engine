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
# LuaExport cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})

IF(ISIXE_WITH_RPG)
	include_directories(${I6ENGINE_RPG_INCLUDE_DIR})
ENDIF(ISIXE_WITH_RPG)

set(srcdir .)

set(luaExportSrc
	${srcdir}/LuaExport.cpp

	${srcdir}/src/LuaAPIExport.cpp
	${srcdir}/src/LuaAudioExport.cpp
	${srcdir}/src/LuaCoreExport.cpp
	${srcdir}/src/LuaGraphicsExport.cpp
	${srcdir}/src/LuaGUIExport.cpp
	${srcdir}/src/LuaInputExport.cpp
	${srcdir}/src/LuaMathExport.cpp
	${srcdir}/src/LuaMessagingExport.cpp
	${srcdir}/src/LuaNetworkExport.cpp
	${srcdir}/src/LuaObjectExport.cpp
	${srcdir}/src/LuaPhysicsExport.cpp
	${srcdir}/src/LuaUtilsExport.cpp
)

IF(ISIXE_WITH_RPG)
	set(luaExportSrc
		${luaExportSrc}

		${srcdir}/src/LuaRPGExport.cpp
	)
ENDIF(ISIXE_WITH_RPG)

add_library(ScriptingLua SHARED ${luaExportSrc})

target_link_libraries(ScriptingLua i6engine-modules)

IF(ISIXE_WITH_RPG)
	target_link_libraries(ScriptingLua i6engine-rpg)
ENDIF(ISIXE_WITH_RPG)

# to find the dependency libs during runtime
set_target_properties(ScriptingLua PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
set_target_properties(ScriptingLua PROPERTIES POSITION_INDEPENDENT_CODE ON)

IF(WIN32)
	add_custom_command(TARGET ScriptingLua POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ScriptingLua.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

########################
# LuaExport install
########################

install(TARGETS ScriptingLua
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
