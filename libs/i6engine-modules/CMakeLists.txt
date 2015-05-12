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
# i6gengine-modules cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_PARTICLEUNIVERSE_INCLUDE_DIR})

set(srcdir ./src)
set(configdir ${CMAKE_SOURCE_DIR}/config)

# List of all component classes
# All these classes will be registered as Components
set(i6components
	${srcdir}/api/components/BillboardComponent.cpp
	${srcdir}/api/components/CameraComponent.cpp
	${srcdir}/api/components/FollowComponent.cpp
	${srcdir}/api/components/LifetimeComponent.cpp
	${srcdir}/api/components/LuminousAppearanceComponent.cpp
	${srcdir}/api/components/MeshAppearanceComponent.cpp
	${srcdir}/api/components/MovableTextComponent.cpp
	${srcdir}/api/components/MoveComponent.cpp
	${srcdir}/api/components/MoverCircleComponent.cpp
	${srcdir}/api/components/MoverInterpolateComponent.cpp
	${srcdir}/api/components/MovingCameraComponent.cpp
	${srcdir}/api/components/ParticleEmitterComponent.cpp
	${srcdir}/api/components/PhysicalStateComponent.cpp
	${srcdir}/api/components/Point2PointConstraintComponent.cpp
	${srcdir}/api/components/SoundComponent.cpp
	${srcdir}/api/components/SoundListenerComponent.cpp
	${srcdir}/api/components/SpawnpointComponent.cpp
	${srcdir}/api/components/StaticStateComponent.cpp
	${srcdir}/api/components/TerrainAppearanceComponent.cpp
	${srcdir}/api/components/ToggleWaynetComponent.cpp
	${srcdir}/api/components/WaynetNavigationComponent.cpp
	${srcdir}/api/components/WaypointComponent.cpp
)

# Generates a file that registers the components
FILE(WRITE ${CMAKE_BINARY_DIR}/include/i6engine/i6eComponentRegister.h "")
SET(componentListHeader "#ifndef __I6ENGINE_API_COMPONENTREGISTER_H__\n#define __I6ENGINE_API_COMPONENTREGISTER_H__\n\n#include <vector>\n\n")
FILE(APPEND ${CMAKE_BINARY_DIR}/include/i6engine/i6eComponentRegister.h "${componentListHeader}")
SET(componentListNamespace "namespace i6engine {\nnamespace api {\n\n\tstd::vector<std::pair<std::string, createGOCCallback>> componentList = {\n")
SET(firstIt TRUE)
FOREACH(comp ${i6components})
	STRING(REGEX MATCH ".*/([a-zA-Z0-9_]+)Component.cpp" compName ${comp})
	FILE(APPEND ${CMAKE_BINARY_DIR}/include/i6engine/i6eComponentRegister.h "#include \"i6engine/api/components/${CMAKE_MATCH_1}Component.h\"\n")
	IF(${firstIt})
		SET(componentListNamespace "${componentListNamespace}\t\t{\"${CMAKE_MATCH_1}\", boost::bind(&${CMAKE_MATCH_1}Component::createC, _1, _2)}")
		SET(firstIt FALSE)
	ELSE()
		SET(componentListNamespace "${componentListNamespace},\n\t\t{\"${CMAKE_MATCH_1}\", boost::bind(&${CMAKE_MATCH_1}Component::createC, _1, _2)}")
	ENDIF()
ENDFOREACH()
FILE(APPEND ${CMAKE_BINARY_DIR}/include/i6engine/i6eComponentRegister.h "\n${componentListNamespace}\n\t};\n\n} /* namespace api */\n} /* namespace i6engine */\n\n#endif /* __I6ENGINE_API_COMPONENTREGISTER_H__ */\n")

# Modules source files
set(i6modulesSrc
	${i6modulesSrc}

	${srcdir}/api/EngineController.cpp
	${srcdir}/api/GameMessage.cpp

	${i6components}
	# These Classes are superclasses and don't need to be registered
	${srcdir}/api/components/Component.cpp
	${srcdir}/api/components/MovementComponent.cpp
	${srcdir}/api/components/MoverComponent.cpp
	${srcdir}/api/components/NavigationComponent.cpp
	${srcdir}/api/components/ShatterComponent.cpp

	${srcdir}/api/configs/AudioConfig.cpp
	${srcdir}/api/configs/ComponentConfig.cpp
	${srcdir}/api/configs/GraphicsConfig.cpp
	${srcdir}/api/configs/GUIConfig.cpp
	${srcdir}/api/configs/InputConfig.cpp
	${srcdir}/api/configs/NetworkConfig.cpp
	${srcdir}/api/configs/ObjectConfig.cpp
	${srcdir}/api/configs/PhysicsConfig.cpp
	${srcdir}/api/configs/ScriptingConfig.cpp

	${srcdir}/api/facades/AudioFacade.cpp
	${srcdir}/api/facades/GraphicsFacade.cpp
	${srcdir}/api/facades/GUIFacade.cpp
	${srcdir}/api/facades/InputFacade.cpp
	${srcdir}/api/facades/MessageSubscriberFacade.cpp
	${srcdir}/api/facades/MessagingFacade.cpp
	${srcdir}/api/facades/NetworkFacade.cpp
	${srcdir}/api/facades/ObjectFacade.cpp
	${srcdir}/api/facades/PhysicsFacade.cpp
	${srcdir}/api/facades/ScriptingFacade.cpp

	${srcdir}/api/gui/GUIWidget.cpp

	${srcdir}/api/manager/IDManager.cpp
	${srcdir}/api/manager/LanguageManager.cpp
	${srcdir}/api/manager/TextManager.cpp
	${srcdir}/api/manager/WaynetManager.cpp

	${srcdir}/api/objects/GameObject.cpp

	${srcdir}/modules/graphics/Debug.cpp
	${srcdir}/modules/graphics/GraphicsController.cpp
	${srcdir}/modules/graphics/GraphicsMailbox.cpp
	${srcdir}/modules/graphics/GraphicsManager.cpp
	${srcdir}/modules/graphics/GraphicsNode.cpp
	${srcdir}/modules/graphics/ResourceManager.cpp
	${srcdir}/modules/graphics/Terrain.cpp
	${srcdir}/modules/graphics/compositors/GaussianBlurListener.cpp
	${srcdir}/modules/graphics/compositors/GaussianBlurLogic.cpp
	${srcdir}/modules/graphics/compositors/HeatVisionListener.cpp
	${srcdir}/modules/graphics/compositors/HeatVisionLogic.cpp
	${srcdir}/modules/graphics/compositors/HDRListener.cpp
	${srcdir}/modules/graphics/compositors/HDRLogic.cpp
	${srcdir}/modules/graphics/graphicswidgets/MovableText.cpp
	
	${srcdir}/modules/gui/GUICanvas.cpp
	${srcdir}/modules/gui/GUIController.cpp
	${srcdir}/modules/gui/GUIFactory.cpp
	${srcdir}/modules/gui/GUIMailbox.cpp
	${srcdir}/modules/gui/GUIManager.cpp
	${srcdir}/modules/gui/guiwidgets/GUIBar.cpp
	${srcdir}/modules/gui/guiwidgets/GUIChat.cpp
	${srcdir}/modules/gui/guiwidgets/GUIComboBox.cpp
	${srcdir}/modules/gui/guiwidgets/GUIEditbox.cpp
	${srcdir}/modules/gui/guiwidgets/GUIImage.cpp
	${srcdir}/modules/gui/guiwidgets/GUIImageButton.cpp
	${srcdir}/modules/gui/guiwidgets/GUIMultiColumnList.cpp
	${srcdir}/modules/gui/guiwidgets/GUIPrint.cpp
	${srcdir}/modules/gui/guiwidgets/GUIStatusList.cpp
	${srcdir}/modules/gui/guiwidgets/GUITextButton.cpp
	${srcdir}/modules/gui/guiwidgets/GUIToggleButton.cpp
	${srcdir}/modules/gui/guiwidgets/GUITooltip.cpp

	${srcdir}/modules/input/InputController.cpp
	${srcdir}/modules/input/InputMailbox.cpp
	${srcdir}/modules/input/InputManager.cpp
	${srcdir}/modules/input/KeyboardListener.cpp
	${srcdir}/modules/input/MouseListener.cpp

	${srcdir}/modules/object/ComponentFactory.cpp
	${srcdir}/modules/object/GOFactory.cpp
	${srcdir}/modules/object/ObjectController.cpp
	${srcdir}/modules/object/ObjectMailbox.cpp
	${srcdir}/modules/object/ObjectManager.cpp

	${srcdir}/modules/physics/DebugDrawer.cpp
	${srcdir}/modules/physics/PhysicsController.cpp
	${srcdir}/modules/physics/PhysicsMailbox.cpp
	${srcdir}/modules/physics/PhysicsManager.cpp
	${srcdir}/modules/physics/PhysicsNode.cpp
)

IF(ISIXE_WITH_AUDIO)
	set(i6modulesSrc
		${i6modulesSrc}

		${srcdir}/modules/audio/AudioController.cpp
		${srcdir}/modules/audio/AudioMailbox.cpp
		${srcdir}/modules/audio/AudioManager.cpp
		${srcdir}/modules/audio/AudioNode.cpp
		${srcdir}/modules/audio/WavFile.cpp
	)
ENDIF(ISIXE_WITH_AUDIO)

IF(ISIXE_NETWORK)
	set(i6modulesSrc
		${i6modulesSrc}

		${srcdir}/api/components/NetworkSenderComponent.cpp

		${srcdir}/modules/network/NetworkController.cpp
		${srcdir}/modules/network/NetworkErrors.cpp
		${srcdir}/modules/network/NetworkMailbox.cpp
		${srcdir}/modules/network/NetworkManager.cpp
	)
ENDIF(ISIXE_NETWORK)

IF(ISIXE_WITH_LUA_SCRIPTING)
	include_directories(${I6ENGINE_LUA_INCLUDE_DIR})
	include_directories(${I6ENGINE_LUABIND_INCLUDE_DIR})

	set(i6modulesSrc
		${i6modulesSrc}

		${srcdir}/modules/luascripting/LuaScriptingController.cpp
		${srcdir}/modules/luascripting/LuaScriptingMailbox.cpp
		${srcdir}/modules/luascripting/LuaScriptingManager.cpp
	)
ENDIF(ISIXE_WITH_LUA_SCRIPTING)

IF(ISIXE_WITH_PYTHON_SCRIPTING)
	set(i6modulesSrc
		${i6modulesSrc}

		${srcdir}/modules/scripting/ScriptingController.cpp
		${srcdir}/modules/scripting/ScriptingMailbox.cpp
		${srcdir}/modules/scripting/ScriptingManager.cpp
	)
ENDIF(ISIXE_WITH_PYTHON_SCRIPTING)

# SOURCE_GROUP required for file grouping in Visual Studio
source_group(api\\components /api/components/*)
source_group(api\\configs /api/configs/*)
source_group(api\\facades /api/facades/*)
source_group(api\\gui /api/gui/*)
source_group(api\\manager /api/manager/*)
source_group(api\\objects /api/objects/*)
source_group(api /api/*)
source_group(modules\\audio /modules/audio/*)
source_group(modules\\graphics /modules/graphics/*)
source_group(modules\\graphics\\compositors /modules/graphics/compositors/*)
source_group(modules\\graphics\\widgets /modules/graphics/graphicswidgets/*)
source_group(modules\\gui /modules/gui/*)
source_group(modules\\gui\\widgets /modules/gui/guiwidgets/*)
source_group(modules\\input /modules/input/*)
source_group(modules\\network /modules/network/*)
source_group(modules\\object /modules/object/*)
source_group(modules\\physics /modules/physics/*)
source_group(modules\\luascripting /modules/luascripting/*)
source_group(modules\\scripting /modules/scripting/*)

IF(ISIXE_BUILD_SHARED)
	add_library(i6engine-modules SHARED ${i6modulesSrc})
ELSE(ISIXE_BUILD_SHARED)
	add_library(i6engine-modules STATIC ${i6modulesSrc})
ENDIF(ISIXE_BUILD_SHARED)

set_target_properties(i6engine-modules PROPERTIES LINKER_LANGUAGE CXX)
# to find the dependency libs during runtime
set_target_properties(i6engine-modules PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

target_link_libraries(i6engine-modules i6engine-utils i6engine-math i6engine-core Plugin_ParticleUniverse ${BULLET_LIBRARIES} ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${TinyXML_LIBRARIES} ${CLOCKUTILS_LIBRARIES})

IF(ISIXE_WITH_AUDIO)
	target_link_libraries(i6engine-modules ${OPENALSOFT_LIBRARIES})
ENDIF(ISIXE_WITH_AUDIO)

IF(ISIXE_WITH_LUA_SCRIPTING)
	target_link_libraries(i6engine-modules lua luabind)
ENDIF(ISIXE_WITH_LUA_SCRIPTING)

IF(ISIXE_WITH_PYTHON_SCRIPTING)
	target_link_libraries(i6engine-modules ${PYTHON_LIBRARIES})
ENDIF(ISIXE_WITH_PYTHON_SCRIPTING)

IF(ISIXE_NETWORK)
	target_link_libraries(i6engine-modules ${M2etis_LIBRARIES})
ENDIF(ISIXE_NETWORK)

IF(WIN32)
	target_link_libraries(i6engine-modules ws2_32)
ENDIF(WIN32)

IF(WIN32 AND ISIXE_BUILD_SHARED)
	add_custom_command(TARGET i6engine-modules POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/i6engine-modules.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32 AND ISIXE_BUILD_SHARED)

########################
# i6engine-modules tests
########################

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS i6engine-modules
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
