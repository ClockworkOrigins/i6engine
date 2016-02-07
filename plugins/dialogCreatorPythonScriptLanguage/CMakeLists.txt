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
# dialogCreatorPythonScriptLanguage plugin cmake
################################

include_directories("include")
include_directories(${CMAKE_SOURCE_DIR}/tools/dialogCreator/include/plugins)

set(srcdir ./src)
SET(includedir ./include/)

set(plugindialogCreatorPythonScriptLanguageSrc
	${srcdir}/DialogCreatorPythonScriptLanguage.cpp
)

QT5_WRAP_CPP(plugindialogCreatorPythonScriptLanguageInclude
	${includedir}/DialogCreatorPythonScriptLanguage.h
)

ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

add_library(Plugin_PythonScriptLanguage SHARED ${plugindialogCreatorPythonScriptLanguageSrc} ${plugindialogCreatorPythonScriptLanguageInclude})

target_link_libraries(Plugin_PythonScriptLanguage ${QT_LIBRARIES})

add_custom_command(TARGET Plugin_PythonScriptLanguage PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages)

IF(WIN32)
	add_custom_command(TARGET Plugin_PythonScriptLanguage POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_PythonScriptLanguage.dll ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages)
ELSEIF(UNIX)
	set_target_properties(Plugin_PythonScriptLanguage PROPERTIES PREFIX "")
	add_custom_command(TARGET Plugin_PythonScriptLanguage POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink Plugin_PythonScriptLanguage.so ${CMAKE_BINARY_DIR}/bin/plugins/dialogCreator/scriptLanguages/Plugin_PythonScriptLanguage.so)
ENDIF(WIN32)

install(TARGETS Plugin_PythonScriptLanguage
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/plugins/dialogCreator/scriptLanguages
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
