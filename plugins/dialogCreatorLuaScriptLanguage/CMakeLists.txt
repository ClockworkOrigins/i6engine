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
# dialogCreatorLuaScriptLanguage plugin cmake
################################

include_directories("include")
include_directories(${CMAKE_SOURCE_DIR}/tools/DialogCreator/include/plugins)

set(srcdir ./src)
SET(includedir ./include/)

set(plugindialogCreatorLuaScriptLanguageSrc
	${srcdir}/DialogCreatorLuaScriptLanguage.cpp
)

QT5_WRAP_CPP(plugindialogCreatorLuaScriptLanguageInclude
	${includedir}/DialogCreatorLuaScriptLanguage.h
)

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

add_library(Plugin_LuaScriptLanguage SHARED ${plugindialogCreatorLuaScriptLanguageSrc} ${plugindialogCreatorLuaScriptLanguageInclude})

target_link_libraries(Plugin_LuaScriptLanguage ${QT_LIBRARIES})

add_custom_command(TARGET Plugin_LuaScriptLanguage PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages)

IF(WIN32)
	add_custom_command(TARGET Plugin_LuaScriptLanguage POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_LuaScriptLanguage.dll ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages)
ELSEIF(UNIX)
	set_target_properties(Plugin_LuaScriptLanguage PROPERTIES PREFIX "")
	add_custom_command(TARGET Plugin_LuaScriptLanguage POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink Plugin_LuaScriptLanguage.so ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages/Plugin_LuaScriptLanguage.so)
ENDIF(WIN32)

install(TARGETS Plugin_LuaScriptLanguage
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/plugins/dialogCreator/scriptLanguages
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
