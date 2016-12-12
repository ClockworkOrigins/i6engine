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

#####################################################################################
# i6engine-editor
#####################################################################################

INCLUDE_DIRECTORIES(common/include)

add_subdirectory(common)

IF(ISIXE_WITH_EDITOR)
	add_subdirectory(i6engine-editor)
	set(I6ENGINE_EDITOR_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/tools/i6engine-editor/include)
ENDIF(ISIXE_WITH_EDITOR)

IF(ISIXE_WITH_RPG)
	add_subdirectory(DialogCreator)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(includeGraphBuilder)
add_subdirectory(meshPreview)
add_subdirectory(meshToBulletConverter)

IF(ISIXE_WITH_RPG)
	add_subdirectory(NPCCreator)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(packager)

IF(ISIXE_WITH_PARTICLEUNIVERSE)
	add_subdirectory(particleEditor)
ENDIF(ISIXE_WITH_PARTICLEUNIVERSE)

IF(ISIXE_WITH_RPG)
	add_subdirectory(TakeControl)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(terrainShapeCreator)
add_subdirectory(terrainTextureConverter)