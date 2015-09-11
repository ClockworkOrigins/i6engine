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
# ParticleUniverseEditor cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/plugins/particleUniverse/include)

set(srcdir ./src)

set(particleUniverseEditorSrc
	${srcdir}/ParticleUniverseAffectorPropertyWindow.cpp
	${srcdir}/ParticleUniverseAnimationWindow.cpp
	${srcdir}/ParticleUniverseBehaviourPropertyWindow.cpp
	${srcdir}/ParticleUniverseConfigDialog.cpp
	${srcdir}/ParticleUniverseConfigFile.cpp
	${srcdir}/ParticleUniverseConnector.cpp
	${srcdir}/ParticleUniverseContextMenu.cpp
	${srcdir}/ParticleUniverseControlPoint.cpp
	${srcdir}/ParticleUniverseControlPointColourControl.cpp
	${srcdir}/ParticleUniverseControlPointControl.cpp
	${srcdir}/ParticleUniverseControlPointDialog.cpp
	${srcdir}/ParticleUniverseEditCanvas.cpp
	${srcdir}/ParticleUniverseEditComponent.cpp
	${srcdir}/ParticleUniverseEditor.cpp
	${srcdir}/ParticleUniverseEditorListener.cpp
	${srcdir}/ParticleUniverseEditTab.cpp
	${srcdir}/ParticleUniverseEditToolbar.cpp
	${srcdir}/ParticleUniverseEmitterPropertyWindow.cpp
	${srcdir}/ParticleUniverseEventHandlerPropertyWindow.cpp
	${srcdir}/ParticleUniverseExternPropertyWindow.cpp
	${srcdir}/ParticleUniverseFileDropTarget.cpp
	${srcdir}/ParticleUniverseGizmo.cpp
	${srcdir}/ParticleUniverseGizmoManager.cpp
	${srcdir}/ParticleUniverseGizmoMovement.cpp
	${srcdir}/ParticleUniverseGizmoOrthoCamera.cpp
	${srcdir}/ParticleUniverseGizmoRotation.cpp
	${srcdir}/ParticleUniverseGizmoScaling.cpp
	${srcdir}/ParticleUniverseMainToolbar.cpp
	${srcdir}/ParticleUniverseMaterialListBox.cpp
	${srcdir}/ParticleUniverseMaterialSelector.cpp
	${srcdir}/ParticleUniverseMaterialTab.cpp
	${srcdir}/ParticleUniverseMediaTypeComboBox.cpp
	${srcdir}/ParticleUniverseMeshSelector.cpp
	${srcdir}/ParticleUniverseObserverPropertyWindow.cpp
	${srcdir}/ParticleUniverseOgreControlComponent.cpp
	${srcdir}/ParticleUniversePropertyWindow.cpp
	${srcdir}/ParticleUniverseRecorder.cpp
	${srcdir}/ParticleUniverseRendererPropertyWindow.cpp
	${srcdir}/ParticleUniverseSliderDialog.cpp
	${srcdir}/ParticleUniverseSpinCtrl.cpp
	${srcdir}/ParticleUniverseSystemPropertyWindow.cpp
	${srcdir}/ParticleUniverseSystemTreeControl.cpp
	${srcdir}/ParticleUniverseTechniquePropertyWindow.cpp
	${srcdir}/ParticleUniverseTextControl.cpp
	${srcdir}/ParticleUniverseUtils.cpp

	${srcdir}/AffectorPropertyWindow/AlignAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/BoxColliderPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/CollisionAvoidanceAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/ColourAffectorDialog.cpp
	${srcdir}/AffectorPropertyWindow/ColourAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/FlockCenteringAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/ForceFieldAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/GeometryRotatorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/GravityAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/InterParticleColliderPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/JetAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/LineAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/LinearForceAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/ParticleFollowerPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/PathFollowerPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/PlaneColliderPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/RandomiserPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/ScaleAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/ScaleVelocityAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/SineForceAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/SphereColliderPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/TextureAnimatorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/TextureRotatorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/VelocityMatchingAffectorPropertyWindow.cpp
	${srcdir}/AffectorPropertyWindow/VortexAffectorPropertyWindow.cpp

	${srcdir}/BehaviourPropertyWindow/SlaveBehaviourPropertyWindow.cpp

	${srcdir}/EmitterPropertyWindow/BoxEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/CircleEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/LineEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/MeshSurfaceEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/PointEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/PositionEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/SlaveEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/SphereSurfaceEmitterPropertyWindow.cpp
	${srcdir}/EmitterPropertyWindow/VertexEmitterPropertyWindow.cpp

	${srcdir}/EventHandlerPropertyWindow/DoAffectorEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoEnableComponentEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoExpireEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoFreezeEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoPlacementParticleEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoScaleEventHandlerPropertyWindow.cpp
	${srcdir}/EventHandlerPropertyWindow/DoStopSystemEventHandlerPropertyWindow.cpp

	${srcdir}/ExternPropertyWindow/BoxColliderExternPropertyWindow.cpp
	${srcdir}/ExternPropertyWindow/GravityExternPropertyWindow.cpp
	${srcdir}/ExternPropertyWindow/PhysXActorExternPropertyWindow.cpp
	${srcdir}/ExternPropertyWindow/PhysXFluidExternPropertyWindow.cpp
	${srcdir}/ExternPropertyWindow/SphereColliderExternPropertyWindow.cpp
	${srcdir}/ExternPropertyWindow/VortexExternPropertyWindow.cpp

	${srcdir}/ObserverPropertyWindow/OnClearObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnCollisionObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnCountObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnEmissionObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnEventFlagObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnExpireObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnPositionObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnQuotaObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnRandomObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnTimeObserverPropertyWindow.cpp
	${srcdir}/ObserverPropertyWindow/OnVelocityObserverPropertyWindow.cpp

	${srcdir}/ogre/app.cpp
	${srcdir}/ogre/control.cpp
	${srcdir}/ogre/rendersystem.cpp
	${srcdir}/ogre/resources.cpp
	${srcdir}/ogre/utils.cpp

	${srcdir}/propgrid/advprops.cpp
	${srcdir}/propgrid/extras.cpp
	${srcdir}/propgrid/manager.cpp
	${srcdir}/propgrid/odcombo.cpp
	${srcdir}/propgrid/propgrid.cpp
	${srcdir}/propgrid/props.cpp

	${srcdir}/RendererPropertyWindow/BeamRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/BillboardRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/BoxRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/EntityRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/LightRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/RibbonTrailRendererPropertyWindow.cpp
	${srcdir}/RendererPropertyWindow/SphereRendererPropertyWindow.cpp
)

IF(WIN32)
	set(particleUniverseEditorSrc
		${particleUniverseEditorSrc}

		${srcdir}/main.cpp
		)
ENDIF(WIN32)

source_group(AffectorPropertyWindow /AffectorPropertyWindow/*)
source_group(BehaviourPropertyWindow /BehaviourPropertyWindow/*)
source_group(EmitterPropertyWindow /EmitterPropertyWindow/*)
source_group(EventHandlerPropertyWindow /EventHandlerPropertyWindow/*)
source_group(ExternPropertyWindow /ExternPropertyWindow/*)
source_group(ObserverPropertyWindow /ObserverPropertyWindow/*)
source_group(ogre /ogre/*)
source_group(propgrid /propgrid/*)
source_group(RendererPropertyWindow /RendererPropertyWindow/*)

add_executable(ParticleUniverseEditor ${particleUniverseEditorSrc})

target_link_libraries(ParticleUniverseEditor Plugin_ParticleUniverse ${OGRE_LIBRARIES} ${WXWIDGETS_LIBRARIES})
IF(UNIX)
	target_compile_options(ParticleUniverseEditor PRIVATE "-D__WXGTK__")
	target_link_libraries(ParticleUniverseEditor ${GTK2_LIBRARIES} ${GLIB2_LIBRARIES} jpeg png pthread X11 z)
	set_target_properties(ParticleUniverseEditor PROPERTIES POSITION_INDEPENDENT_CODE ON)
ELSEIF(WIN32)
	target_link_libraries(ParticleUniverseEditor comctl32 rpcrt4)
	SET_TARGET_PROPERTIES(ParticleUniverseEditor PROPERTIES COMPILE_FLAGS "-DUNICODE -DwxUSE_UNICODE=1 -D_UNICODE")
ENDIF()

IF(WIN32)
	add_custom_command(TARGET ParticleUniverseEditor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/ParticleUniverseEditor.exe ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(TARGETS ParticleUniverseEditor
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
