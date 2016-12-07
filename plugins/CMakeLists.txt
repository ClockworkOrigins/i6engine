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
# ParticleUniverse
#####################################################################################

IF(ISIXE_WITH_TOOLS AND ISIXE_WITH_EDITOR)
	IF(ISIXE_WITH_RPG)
		ADD_SUBDIRECTORY(dialogCreatorLuaScriptLanguage)
		ADD_SUBDIRECTORY(dialogCreatorPythonScriptLanguage)
		ADD_SUBDIRECTORY(editorRPGInitialization)
		ADD_SUBDIRECTORY(editorRPGNPCObjectType)
		ADD_SUBDIRECTORY(editorRPGRunGame)
		IF(SCRIPTING_LUA OR SCRIPTING_PYTHON)
			ADD_SUBDIRECTORY(editorLoadLevelRPG)
			ADD_SUBDIRECTORY(editorSaveObjectRPG)
		ENDIF(SCRIPTING_LUA OR SCRIPTING_PYTHON)
	ENDIF(ISIXE_WITH_RPG)
	add_subdirectory(editorTypeAngle)
	add_subdirectory(editorTypeBool)
	add_subdirectory(editorTypeCollisionFlags)
	add_subdirectory(editorTypeColour)
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
ENDIF(ISIXE_WITH_TOOLS AND ISIXE_WITH_EDITOR)
IF(ISIXE_WITH_TOOLS)
	IF(ISIXE_WITH_RPG)
		add_subdirectory(takeControlRPG)
	ENDIF(ISIXE_WITH_RPG)
ENDIF(ISIXE_WITH_TOOLS)

add_subdirectory(i6Archive)
IF(ISIXE_WITH_PARTICLEUNIVERSE)
	add_subdirectory(particleUniverse)
ENDIF(ISIXE_WITH_PARTICLEUNIVERSE)