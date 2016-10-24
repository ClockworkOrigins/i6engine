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
# toolsCommon cmake
################################

INCLUDE_DIRECTORIES(${I6ENGINE_UTILS_INCLUDE_DIR})

set(srcdir ./src)
SET(includedir ./include)

set(toolsCommonSrc
	${srcdir}/Config.cpp

	${srcdir}/widgets/AboutDialog.cpp
)

QT5_WRAP_CPP(toolsCommonGuiHeader
	${includedir}/widgets/AboutDialog.h
)

SOURCE_GROUP(moc /moc_*)
source_group(widgets /widgets/*)

add_library(toolsCommon STATIC ${toolsCommonSrc} ${toolsCommonGuiHeader})
set_target_properties(toolsCommon PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(toolsCommon ${QT_LIBRARIES})

install(TARGETS toolsCommon
		DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
	)
