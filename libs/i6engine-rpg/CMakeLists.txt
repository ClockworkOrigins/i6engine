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
# i6gengine-rpg cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

IF(SCRIPTING_LUA)
	include_directories(${I6ENGINE_LUA_INCLUDE_DIR})
	include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})
ENDIF(SCRIPTING_LUA)

set(srcdir ./src)

set(i6rpgSrc
	${srcdir}/RPGApplication.cpp

	${srcdir}/components/AttributeComponent.cpp
	${srcdir}/components/DialogCheckerComponent.cpp
	${srcdir}/components/ExperienceComponent.cpp
	${srcdir}/components/HealthbarComponent.cpp
	${srcdir}/components/HumanMovementComponent.cpp
	${srcdir}/components/InventoryComponent.cpp
	${srcdir}/components/ItemComponent.cpp
	${srcdir}/components/ListInventoryComponent.cpp
	${srcdir}/components/MiscItemComponent.cpp
	${srcdir}/components/NameComponent.cpp
	${srcdir}/components/QuickslotComponent.cpp
	${srcdir}/components/SlotComponent.cpp
	${srcdir}/components/SlotInventoryComponent.cpp
	${srcdir}/components/ThirdPersonControlComponent.cpp
	${srcdir}/components/UsableItemComponent.cpp
	${srcdir}/components/WeightInventoryComponent.cpp
	
	${srcdir}/config/ExternalConstants.cpp
	
	${srcdir}/dialog/Dialog.cpp
	${srcdir}/dialog/DialogManager.cpp
	${srcdir}/dialog/DialogParser.cpp
	
	${srcdir}/gui/SubtitleWidget.cpp
	
	${srcdir}/item/ItemManager.cpp
	${srcdir}/item/ItemParser.cpp
	
	${srcdir}/npc/NPC.cpp
	${srcdir}/npc/NPCManager.cpp
	${srcdir}/npc/NPCParser.cpp
	${srcdir}/npc/NPCQueue.cpp
	
	${srcdir}/npc/queueJobs/ExitDialogJob.cpp
	${srcdir}/npc/queueJobs/SayJob.cpp
	${srcdir}/npc/queueJobs/ShowDialogsJob.cpp
	${srcdir}/npc/queueJobs/StartTradeJob.cpp
	${srcdir}/npc/queueJobs/TurnToNpcJob.cpp
	${srcdir}/npc/queueJobs/WaitSayJob.cpp
	
	${srcdir}/quest/Quest.cpp
	${srcdir}/quest/QuestLog.cpp
	${srcdir}/quest/QuestParser.cpp
)

SOURCE_GROUP(rpg\\components src/components/*)
SOURCE_GROUP(rpg\\config src/config/*)
SOURCE_GROUP(rpg\\dialog src/dialog/*)
SOURCE_GROUP(rpg\\gui src/gui/*)
SOURCE_GROUP(rpg\\item src/item/*)
SOURCE_GROUP(rpg\\npc src/npc/*)
SOURCE_GROUP(rpg\\npc\\queueJobs src/npc/queueJobs/*)
SOURCE_GROUP(rpg\\quest src/quest/*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-rpg SHARED ${i6rpgSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-rpg STATIC ${i6rpgSrc})
ENDIF(ISIXE_BUILD_SHARED)

target_link_libraries(i6engine-rpg i6engine-modules ${BOOST_LIBRARIES})
# to find the dependency libs during runtime
set_target_properties(i6engine-rpg PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-rpg POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-rpg.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

set_target_properties(i6engine-rpg PROPERTIES POSITION_INDEPENDENT_CODE ON)

########################
# i6engine-rpg install
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-rpg
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
