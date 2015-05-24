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
