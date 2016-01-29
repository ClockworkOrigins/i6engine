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
# i6engine-editor
#####################################################################################

IF(ISIXE_WITH_EDITOR)
	add_subdirectory(i6engine-editor)
	set(I6ENGINE_EDITOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/i6engine-editor/include)
ENDIF(ISIXE_WITH_EDITOR)

IF(ISIXE_WITH_RPG)
	add_subdirectory(DialogCreator)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(includeGraphBuilder)
add_subdirectory(meshToBulletConverter)

IF(ISIXE_WITH_RPG)
	add_subdirectory(NPCCreator)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(packager)
add_subdirectory(particleEditor)
add_subdirectory(particleUniverseEditor)
add_subdirectory(terrainShapeCreator)
add_subdirectory(terrainTextureConverter)