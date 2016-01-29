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

#####################################################################################
# ParticleUniverse
#####################################################################################

IF(ISIXE_WITH_TOOLS AND ISIXE_WITH_EDITOR AND ISIXE_WITH_RPG)
	ADD_SUBDIRECTORY(editorRPGInitialization)
	ADD_SUBDIRECTORY(editorRPGRunGame)
ENDIF()
add_subdirectory(editorTypeAngle)
add_subdirectory(editorTypeBool)
add_subdirectory(editorTypeDouble)
add_subdirectory(editorTypeInteger)
add_subdirectory(editorTypeLightType)
add_subdirectory(editorTypeMoverInterpolateMode)
add_subdirectory(editorTypeMoverInterpolateWay)
add_subdirectory(editorTypeMoverPositioning)
add_subdirectory(editorTypeQuaternion)
add_subdirectory(editorTypeShapeType)
add_subdirectory(editorTypeShatterInterest)
add_subdirectory(editorTypeString)
add_subdirectory(editorTypeVec3)
add_subdirectory(editorTypeVec4)
add_subdirectory(i6Archive)
add_subdirectory(particleUniverse)