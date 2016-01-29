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
# i6Archive plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_EDITOR_INCLUDE_DIR})

set(srcdir ./src)

set(plugini6ArchiveSrc
	${srcdir}/i6Archive.cpp
	${srcdir}/i6ArchiveFactory.cpp
	${srcdir}/i6ArchivePlugin.cpp
)

add_library(Plugin_i6Archive SHARED ${plugini6ArchiveSrc})

target_link_libraries(Plugin_i6Archive ${BOOST_LIBRARIES} ${CLOCKUTILS_LIBRARIES} ${OGRE_LIBRARIES})

IF(UNIX)
	set_target_properties(Plugin_i6Archive PROPERTIES PREFIX "")
	set_target_properties(Plugin_i6Archive PROPERTIES SUFFIX ".so.1.9.0")
ENDIF(UNIX)

IF(WIN32)
	add_custom_command(TARGET Plugin_i6Archive POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_i6Archive.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS Plugin_i6Archive
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
