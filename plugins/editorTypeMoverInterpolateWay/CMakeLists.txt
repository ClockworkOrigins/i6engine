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
# editorTypeMoverInterpolateWay plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${I6ENGINE_EDITOR_INCLUDE_DIR})

set(srcdir ./src)
SET(includedir ./include/)

set(pluginEditorTypeMoverInterpolateWaySrc
	${srcdir}/EditorTypeMoverInterpolateWay.cpp
)

QT5_WRAP_CPP(pluginEditorTypeMoverInterpolateWayInclude
	${includedir}/EditorTypeMoverInterpolateWay.h
)

ADD_DEFINITIONS(-DQT_STATICPLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)

add_library(Plugin_EditorTypeMoverInterpolateWay STATIC ${pluginEditorTypeMoverInterpolateWaySrc} ${pluginEditorTypeMoverInterpolateWayInclude})

target_link_libraries(Plugin_EditorTypeMoverInterpolateWay ${QT_LIBRARIES})
