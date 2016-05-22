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
# PythonUtilsExport cmake
################################

include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})

set(srcdir .)

set(pythonUtilsSrc
	${srcdir}/PythonUtilsExport.cpp
)

PYTHON_ADD_MODULE(ScriptingUtilsPython ${pythonUtilsSrc})
target_link_libraries(ScriptingUtilsPython i6engine-utils ${PYTHON_LIBRARIES})

IF(WIN32)
	add_custom_command(TARGET ScriptingUtilsPython POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ScriptingUtilsPython.pyd ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

#############################
# PythonUtilsExport install
#############################

install(TARGETS ScriptingUtilsPython
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
