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
# PythonAPIExport
#####################################################################################

add_subdirectory(api)

#####################################################################################
# PythonAudioExport
#####################################################################################

add_subdirectory(audio)

#####################################################################################
# PythonCoreExport
#####################################################################################

add_subdirectory(core)

#####################################################################################
# PythonGraphicsExport
#####################################################################################

add_subdirectory(graphics)

#####################################################################################
# PythonGUIExport
#####################################################################################

add_subdirectory(gui)

#####################################################################################
# PythonInputExport
#####################################################################################

add_subdirectory(input)

#####################################################################################
# PythonMathExport
#####################################################################################

add_subdirectory(math)

#####################################################################################
# PythonMessagingExport
#####################################################################################

add_subdirectory(messaging)

#####################################################################################
# PythonNetworkExport
#####################################################################################

IF(ISIXE_WITH_NETWORK)
	add_subdirectory(network)
ENDIF(ISIXE_WITH_NETWORK)

#####################################################################################
# PythonObjectExport
#####################################################################################

add_subdirectory(object)

#####################################################################################
# PythonPhysicsExport
#####################################################################################

add_subdirectory(physics)

#####################################################################################
# PythonRPGExport
#####################################################################################

IF(ISIXE_WITH_RPG)
	add_subdirectory(rpg)
ENDIF(ISIXE_WITH_RPG)

#####################################################################################
# PythonUtilsExport
#####################################################################################

add_subdirectory(utils)
