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

IF(ISIXE_NETWORK)
	add_subdirectory(chat)
ENDIF(ISIXE_NETWORK)

add_subdirectory(dragdrop)
add_subdirectory(jenga)

IF(ISIXE_WITH_EDITOR AND ISIXE_WITH_TOOLS)
	add_subdirectory(leveleditor)
ENDIF(ISIXE_WITH_EDITOR AND ISIXE_WITH_TOOLS)

add_subdirectory(multilanguage)
add_subdirectory(physicspower)

IF(ISIXE_WITH_RPG)
	add_subdirectory(rpg)
ENDIF(ISIXE_WITH_RPG)

add_subdirectory(terrain)
add_subdirectory(tooltip)