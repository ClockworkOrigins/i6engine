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
# editorTypeString plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_EDITOR_INCLUDE_DIR})

set(srcdir ./src)
SET(includedir ./include/)

set(pluginEditorTypeStringSrc
	${srcdir}/EditorTypeString.cpp
)

QT5_WRAP_CPP(pluginEditorTypeStringInclude
	${includedir}/EditorTypeString.h
)

ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)

add_library(Plugin_EditorTypeString STATIC ${pluginEditorTypeStringSrc} ${pluginEditorTypeStringInclude})

target_link_libraries(Plugin_EditorTypeString ${QT_LIBRARIES})
