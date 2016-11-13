##
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
##

IF(WIN32)
	file(GLOB_RECURSE files "${I6ENGINE_DEP_DIR}/*.dll")
	foreach(filename ${files})
		execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${filename} ${CMAKE_BINARY_DIR}/bin)
	endforeach()

	file(GLOB_RECURSE files "$ENV{Qt5_DIR}/bin/icudt*.dll")
	foreach(filename ${files})
		get_filename_component(filename ${filename} NAME)
		configure_file($ENV{Qt5_DIR}/bin/${filename} ${CMAKE_BINARY_DIR}/bin/${filename} COPYONLY)
	endforeach()

	file(GLOB_RECURSE files "$ENV{Qt5_DIR}/bin/icuin*.dll")
	foreach(filename ${files})
		get_filename_component(filename ${filename} NAME)
		configure_file($ENV{Qt5_DIR}/bin/${filename} ${CMAKE_BINARY_DIR}/bin/${filename} COPYONLY)
	endforeach()

	file(GLOB_RECURSE files "$ENV{Qt5_DIR}/bin/icuuc*.dll")
	foreach(filename ${files})
		get_filename_component(filename ${filename} NAME)
		configure_file($ENV{Qt5_DIR}/bin/${filename} ${CMAKE_BINARY_DIR}/bin/${filename} COPYONLY)
	endforeach()

	configure_file($ENV{Qt5_DIR}/bin/libEGL.dll ${CMAKE_BINARY_DIR}/bin/libEGL.dll COPYONLY)
	configure_file($ENV{Qt5_DIR}/bin/libGLESv2.dll ${CMAKE_BINARY_DIR}/bin/libGLESv2.dll COPYONLY)
	configure_file($ENV{Qt5_DIR}/plugins/platforms/qwindows.dll ${CMAKE_BINARY_DIR}/bin/plugins/platforms/qwindows.dll COPYONLY)

	configure_file(${CMAKE_SOURCE_DIR}/config/qt.conf ${CMAKE_BINARY_DIR}/bin/qt.conf COPYONLY)

	configure_file($ENV{Qt5_DIR}/bin/Qt5Core.dll ${CMAKE_BINARY_DIR}/bin/Qt5Core.dll COPYONLY)
	configure_file($ENV{Qt5_DIR}/bin/Qt5Gui.dll ${CMAKE_BINARY_DIR}/bin/Qt5Gui.dll COPYONLY)
	configure_file($ENV{Qt5_DIR}/bin/Qt5Widgets.dll ${CMAKE_BINARY_DIR}/bin/Qt5Widgets.dll COPYONLY)
ENDIF(WIN32)
IF(UNIX)
	file(GLOB_RECURSE files "${I6ENGINE_DEP_DIR}/*.so")
	foreach(filename ${files})
		get_filename_component(filename_pure ${filename} NAME)
		execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filename} ${CMAKE_BINARY_DIR}/bin/${filename_pure})
	endforeach()
ENDIF(UNIX)
