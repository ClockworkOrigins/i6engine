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
# ParticleEditor cmake
################################

INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES(${I6ENGINE_UTILS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_MATH_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_CORE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${I6ENGINE_MODULES_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/plugins/particleUniverse/include)

set(srcdir ./src)
SET(includedir ./include)

set(particleEditorSrc
	${srcdir}/main.cpp
	
	${srcdir}/connections/Connection.cpp
	${srcdir}/connections/LineConnector.cpp
	
	${srcdir}/factories/AffectorPropertyWindowFactory.cpp
	${srcdir}/factories/BehaviourPropertyWindowFactory.cpp
	${srcdir}/factories/EmitterPropertyWindowFactory.cpp
	${srcdir}/factories/EventHandlerPropertyWindowFactory.cpp
	${srcdir}/factories/ExternPropertyWindowFactory.cpp
	${srcdir}/factories/ObserverPropertyWindowFactory.cpp
	${srcdir}/factories/RendererPropertyWindowFactory.cpp
	
	${srcdir}/properties/Property.cpp
	${srcdir}/properties/BoolProperty.cpp
	${srcdir}/properties/ColourWithAlphaProperty.cpp
	${srcdir}/properties/DoubleProperty.cpp
	${srcdir}/properties/DynamicAttributeProperty.cpp
	${srcdir}/properties/EnumProperty.cpp
	${srcdir}/properties/QuaternionProperty.cpp
	${srcdir}/properties/StringProperty.cpp
	${srcdir}/properties/TimeAndColourListProperty.cpp
	${srcdir}/properties/TimeAndColourProperty.cpp
	${srcdir}/properties/UIntProperty.cpp
	${srcdir}/properties/Vec3ListProperty.cpp
	${srcdir}/properties/Vec3Property.cpp
	
	${srcdir}/widgets/AffectorPropertyWindow.cpp
	${srcdir}/widgets/BehaviourPropertyWindow.cpp
	${srcdir}/widgets/DialogChooseConnectionType.cpp
	${srcdir}/widgets/EmitterPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindow.cpp
	${srcdir}/widgets/ExternPropertyWindow.cpp
	${srcdir}/widgets/GraphicsScene.cpp
	${srcdir}/widgets/MainWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindow.cpp
	${srcdir}/widgets/PropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindow.cpp
	${srcdir}/widgets/SystemPropertyWindow.cpp
	${srcdir}/widgets/TechniquePropertyWindow.cpp
	${srcdir}/widgets/WidgetEdit.cpp
	${srcdir}/widgets/WidgetEditComponent.cpp
	${srcdir}/widgets/WidgetParticleList.cpp
	${srcdir}/widgets/WidgetRender.cpp
	${srcdir}/widgets/WidgetScript.cpp
	
	${srcdir}/widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/CollisionAvoidanceAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/FlockCenteringAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/ForceFieldAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/GeometryRotatorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/InterParticleColliderPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/LineAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/LinearForceAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/ParticleFollowerPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/PlaneColliderPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/RandomiserPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/ScaleAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/ScaleVelocityAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/SineForceAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/TextureAnimatorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/VelocityMatchingAffectorPropertyWindow.cpp
	${srcdir}/widgets/AffectorPropertyWindows/VortexAffectorPropertyWindow.cpp
	
	${srcdir}/widgets/BehaviourPropertyWindows/SlaveBehaviourPropertyWindow.cpp
	
	${srcdir}/widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/LineEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/PointEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/SlaveEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.cpp
	${srcdir}/widgets/EmitterPropertyWindows/VertexEmitterPropertyWindow.cpp
	
	${srcdir}/widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoExpireEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoFreezeEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.cpp
	${srcdir}/widgets/EventHandlerPropertyWindows/DoStopSystemEventHandlerPropertyWindow.cpp
	
	${srcdir}/widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.cpp
	${srcdir}/widgets/ExternPropertyWindows/GravityExternPropertyWindow.cpp
	${srcdir}/widgets/ExternPropertyWindows/SphereColliderExternPropertyWindow.cpp
	${srcdir}/widgets/ExternPropertyWindows/VortexExternPropertyWindow.cpp
	
	${srcdir}/widgets/ObserverPropertyWindows/OnClearObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnCollisionObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnEmissionObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnExpireObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnQuotaObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnTimeObserverPropertyWindow.cpp
	${srcdir}/widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.cpp

	${srcdir}/widgets/RendererPropertyWindows/BeamRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/BoxRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/EntityRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/LightRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/RibbonTrailRendererPropertyWindow.cpp
	${srcdir}/widgets/RendererPropertyWindows/SphereRendererPropertyWindow.cpp
)

QT5_WRAP_UI(particleEditorUI
	gui/dialogChooseConnectionType.ui
	gui/widgetEdit.ui
	gui/widgetParticleList.ui
	gui/widgetProperty.ui
	gui/widgetPropertyWindow.ui
	gui/widgetRender.ui
	gui/widgetScript.ui
	gui/wndMainWindow.ui
)

QT5_WRAP_CPP(particleEditorGuiHeader
	${includedir}/connections/LineConnector.h
	
	${includedir}/properties/Property.h
	${includedir}/properties/BoolProperty.h
	${includedir}/properties/ColourWithAlphaProperty.h
	${includedir}/properties/DoubleProperty.h
	${includedir}/properties/DynamicAttributeProperty.h
	${includedir}/properties/EnumProperty.h
	${includedir}/properties/QuaternionProperty.h
	${includedir}/properties/StringProperty.h
	${includedir}/properties/TimeAndColourListProperty.h
	${includedir}/properties/TimeAndColourProperty.h
	${includedir}/properties/UIntProperty.h
	${includedir}/properties/Vec3ListProperty.h
	${includedir}/properties/Vec3Property.h
	
	${includedir}/widgets/AffectorPropertyWindow.h
	${includedir}/widgets/BehaviourPropertyWindow.h
	${includedir}/widgets/DialogChooseConnectionType.h
	${includedir}/widgets/EmitterPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindow.h
	${includedir}/widgets/ExternPropertyWindow.h
	${includedir}/widgets/GraphicsScene.h
	${includedir}/widgets/MainWindow.h
	${includedir}/widgets/ObserverPropertyWindow.h
	${includedir}/widgets/PropertyWindow.h
	${includedir}/widgets/RendererPropertyWindow.h
	${includedir}/widgets/SystemPropertyWindow.h
	${includedir}/widgets/TechniquePropertyWindow.h
	${includedir}/widgets/WidgetEdit.h
	${includedir}/widgets/WidgetEditComponent.h
	${includedir}/widgets/WidgetParticleList.h
	${includedir}/widgets/WidgetRender.h
	${includedir}/widgets/WidgetScript.h
	
	${includedir}/widgets/AffectorPropertyWindows/AlignAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/CollisionAvoidanceAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/ColourAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/FlockCenteringAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/ForceFieldAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/GeometryRotatorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/GravityAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/InterParticleColliderPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/JetAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/LineAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/LinearForceAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/ParticleFollowerPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/PlaneColliderPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/RandomiserPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/ScaleAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/ScaleVelocityAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/SineForceAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/TextureAnimatorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/TextureRotatorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/VelocityMatchingAffectorPropertyWindow.h
	${includedir}/widgets/AffectorPropertyWindows/VortexAffectorPropertyWindow.h
	
	${includedir}/widgets/BehaviourPropertyWindows/SlaveBehaviourPropertyWindow.h
	
	${includedir}/widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/LineEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/PointEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/SlaveEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h
	${includedir}/widgets/EmitterPropertyWindows/VertexEmitterPropertyWindow.h
	
	${includedir}/widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoExpireEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoFreezeEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h
	${includedir}/widgets/EventHandlerPropertyWindows/DoStopSystemEventHandlerPropertyWindow.h
	
	${includedir}/widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h
	${includedir}/widgets/ExternPropertyWindows/GravityExternPropertyWindow.h
	${includedir}/widgets/ExternPropertyWindows/SphereColliderExternPropertyWindow.h
	${includedir}/widgets/ExternPropertyWindows/VortexExternPropertyWindow.h
	
	${includedir}/widgets/ObserverPropertyWindows/OnClearObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnCollisionObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnEmissionObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnExpireObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnQuotaObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnTimeObserverPropertyWindow.h
	${includedir}/widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.h

	${includedir}/widgets/RendererPropertyWindows/BeamRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/BoxRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/EntityRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/LightRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/RibbonTrailRendererPropertyWindow.h
	${includedir}/widgets/RendererPropertyWindows/SphereRendererPropertyWindow.h
)

source_group(connections /connections/*)
source_group(factories /factories/*)
source_group(properties /properties/*)
source_group(widgets /widgets/*)
source_group(widgets\\AffectorPropertyWindows /widgets/AffectorPropertyWindows/*)
source_group(widgets\\BehaviourPropertyWindows /widgets/BehaviourPropertyWindows/*)
source_group(widgets\\EmitterPropertyWindows /widgets/EmitterPropertyWindows/*)
source_group(widgets\\EventHandlerPropertyWindows /widgets/EventHandlerPropertyWindows/*)
source_group(widgets\\ExternPropertyWindows /widgets/ExternPropertyWindows/*)
source_group(widgets\\ObserverPropertyWindows /widgets/ObserverPropertyWindows/*)
source_group(widgets\\RendererPropertyWindows /widgets/RendererPropertyWindows/*)
SOURCE_GROUP(moc /moc_*)

# add translations ...
QT5_ADD_TRANSLATION(QT_TRANSLATION_FILES ${TRANS})

ADD_EXECUTABLE(ParticleEditor ${GUI_TYPE} ../common/src/Config.cpp ${particleEditorSrc} ${particleEditorUI} ${particleEditorGuiHeader} ${QT_TRANSLATION_FILES})

target_link_libraries(ParticleEditor i6engine-modules ${QT_LIBRARIES})

add_custom_command(TARGET ParticleEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/de_DE.qm ${CMAKE_BINARY_DIR}/bin)
add_custom_command(TARGET ParticleEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/en_US.qm ${CMAKE_BINARY_DIR}/bin)

IF(WIN32)
	add_custom_command(TARGET ParticleEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ParticleEditor.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS ParticleEditor
	DESTINATION ${CMAKE_INSTALL_PREFIX}/tools
)
