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
# ParticleUniverse plugin cmake
################################

include_directories("include")
include_directories(${I6ENGINE_UTILS_INCLUDE_DIR})
include_directories(${I6ENGINE_MATH_INCLUDE_DIR})
include_directories(${I6ENGINE_CORE_INCLUDE_DIR})
include_directories(${I6ENGINE_MODULES_INCLUDE_DIR})

set(srcdir ./src)

set(pluginParticleUniverseSrc
	${srcdir}/ParticleUniverseAffector.cpp
	${srcdir}/ParticleUniverseAffectorTokens.cpp
	${srcdir}/ParticleUniverseAliasTokens.cpp
	${srcdir}/ParticleUniverseAtlasImage.cpp
	${srcdir}/ParticleUniverseAttachable.cpp
	${srcdir}/ParticleUniverseAttachableTokens.cpp
	${srcdir}/ParticleUniverseBehaviourTokens.cpp
	${srcdir}/ParticleUniverseCameraDependency.cpp
	${srcdir}/ParticleUniverseCameraDependencyFactory.cpp
	${srcdir}/ParticleUniverseCameraDependencyTokens.cpp
	${srcdir}/ParticleUniverseDll.cpp
	${srcdir}/ParticleUniverseDynamicAttribute.cpp
	${srcdir}/ParticleUniverseDynamicAttributeTokens.cpp
	${srcdir}/ParticleUniverseEmitter.cpp
	${srcdir}/ParticleUniverseEmitterTokens.cpp
	${srcdir}/ParticleUniverseEventHandlerTokens.cpp
	${srcdir}/ParticleUniverseExtern.cpp
	${srcdir}/ParticleUniverseExternTokens.cpp
	${srcdir}/ParticleUniverseForceField.cpp
	${srcdir}/ParticleUniverseNoise.cpp
	${srcdir}/ParticleUniverseObserver.cpp
	${srcdir}/ParticleUniverseObserverTokens.cpp
	${srcdir}/ParticleUniverseParticle.cpp
	${srcdir}/ParticleUniverseParticlePool.cpp
	${srcdir}/ParticleUniversePlugin.cpp
	${srcdir}/ParticleUniverseRenderer.cpp
	${srcdir}/ParticleUniverseRendererTokens.cpp
	${srcdir}/ParticleUniverseScriptDeserializer.cpp
	${srcdir}/ParticleUniverseScriptSerializer.cpp
	${srcdir}/ParticleUniverseSystem.cpp
	${srcdir}/ParticleUniverseSystemManager.cpp
	${srcdir}/ParticleUniverseSystemManagerTokens.cpp
	${srcdir}/ParticleUniverseSystemTokens.cpp
	${srcdir}/ParticleUniverseTechnique.cpp
	${srcdir}/ParticleUniverseTechniqueTokens.cpp
	${srcdir}/ParticleUniverseVisualParticle.cpp

	${srcdir}/Externs/ParticleUniverseBoxColliderExtern.cpp
	${srcdir}/Externs/ParticleUniverseBoxColliderExternTokens.cpp
	${srcdir}/Externs/ParticleUniverseGravityExtern.cpp
	${srcdir}/Externs/ParticleUniverseGravityExternTokens.cpp
	${srcdir}/Externs/ParticleUniversePhysXActorExtern.cpp
	${srcdir}/Externs/ParticleUniversePhysXActorExternTokens.cpp
	${srcdir}/Externs/ParticleUniversePhysXBridge.cpp
	${srcdir}/Externs/ParticleUniversePhysXFluidExtern.cpp
	${srcdir}/Externs/ParticleUniversePhysXFluidExternTokens.cpp
	${srcdir}/Externs/ParticleUniverseSceneDecoratorExtern.cpp
	${srcdir}/Externs/ParticleUniverseSceneDecoratorExternTokens.cpp
	${srcdir}/Externs/ParticleUniverseSphereColliderExtern.cpp
	${srcdir}/Externs/ParticleUniverseSphereColliderExternTokens.cpp
	${srcdir}/Externs/ParticleUniverseVortexExtern.cpp
	${srcdir}/Externs/ParticleUniverseVortexExternTokens.cpp

	${srcdir}/ParticleAffectors/ParticleUniverseAlignAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseAlignAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBaseCollider.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBaseColliderTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBaseForceAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBaseForceAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBoxCollider.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseBoxColliderTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseCollisionAvoidanceAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseColourAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseColourAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseFlockCenteringAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseFlockCenteringAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseForceFieldAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseForceFieldAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseGeometryRotator.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseGeometryRotatorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseGravityAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseGravityAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseInterParticleCollider.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseInterParticleColliderTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseJetAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseJetAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseLineAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseLineAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseLinearForceAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseLinearForceAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseParticleFollower.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseParticleFollowerTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniversePathFollower.cpp
	${srcdir}/ParticleAffectors/ParticleUniversePathFollowerTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniversePlaneCollider.cpp
	${srcdir}/ParticleAffectors/ParticleUniversePlaneColliderTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseRandomiser.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseRandomiserTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseScaleAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseScaleAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseScaleVelocityAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseScaleVelocityAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseSineForceAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseSineForceAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseSphereCollider.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseSphereColliderTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseTextureAnimator.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseTextureAnimatorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseTextureRotator.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseTextureRotatorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseVelocityMatchingAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseVelocityMatchingAffectorTokens.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseVortexAffector.cpp
	${srcdir}/ParticleAffectors/ParticleUniverseVortexAffectorTokens.cpp
	
	${srcdir}/ParticleBehaviours/ParticleUniverseSlaveBehaviour.cpp
	${srcdir}/ParticleBehaviours/ParticleUniverseSlaveBehaviourTokens.cpp
	
	${srcdir}/ParticleEmitters/ParticleUniverseBoxEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseBoxEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseCircleEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseCircleEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseLineEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseLineEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseMeshSurfaceEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseMeshSurfaceEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniversePointEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniversePointEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniversePositionEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniversePositionEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseSlaveEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseSlaveEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseSphereSurfaceEmitterTokens.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseVertexEmitter.cpp
	${srcdir}/ParticleEmitters/ParticleUniverseVertexEmitterTokens.cpp
	
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoAffectorEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoExpireEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoExpireEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoFreezeEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoFreezeEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoScaleEventHandlerTokens.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoStopSystemEventHandler.cpp
	${srcdir}/ParticleEventHandlers/ParticleUniverseDoStopSystemEventHandlerTokens.cpp
	
	${srcdir}/ParticleObservers/ParticleUniverseOnClearObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnClearObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnCollisionObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnCollisionObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnCountObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnCountObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnEmissionObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnEmissionObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnEventFlagObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnEventFlagObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnExpireObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnExpireObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnPositionObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnPositionObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnQuotaObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnQuotaObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnRandomObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnRandomObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnTimeObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnTimeObserverTokens.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnVelocityObserver.cpp
	${srcdir}/ParticleObservers/ParticleUniverseOnVelocityObserverTokens.cpp
	
	${srcdir}/ParticleRenderers/ParticleUniverseBeamRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBeamRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBillboardRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBillboardRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBox.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBoxRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBoxRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseBoxSet.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseEntityRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseEntityRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseLightRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseLightRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniversePrimitiveShapeSet.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseRibbonTrailRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseRibbonTrailRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseSphere.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseSphereRenderer.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseSphereRendererTokens.cpp
	${srcdir}/ParticleRenderers/ParticleUniverseSphereSet.cpp
)

source_group(Externs /Externs/*)
source_group(ParticleAffectors /ParticleAffectors/*)
source_group(ParticleBehaviours /ParticleBehaviours/*)
source_group(ParticleEmitters /ParticleEmitters/*)
source_group(ParticleEventHandlers /ParticleEventHandlers/*)
source_group(ParticleObservers /ParticleObservers/*)
source_group(ParticleRenderers /ParticleRenderers/*)

add_library(Plugin_ParticleUniverse SHARED ${pluginParticleUniverseSrc})

IF(UNIX)
	set_target_properties(Plugin_ParticleUniverse PROPERTIES PREFIX "")
	set_target_properties(Plugin_ParticleUniverse PROPERTIES SUFFIX ".so.1.9.0")

	target_link_libraries(Plugin_ParticleUniverse ${BOOST_LIBRARIES} ${OGRE_LIBRARIES})
ENDIF(UNIX)

IF(WIN32)
	target_link_libraries(Plugin_ParticleUniverse ${BOOST_LIBRARIES} ${OGRE_LIBRARIES})
	add_custom_command(TARGET Plugin_ParticleUniverse POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/Plugin_ParticleUniverse.dll ${CMAKE_BINARY_DIR}/bin)
ENDIF(WIN32)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX})

install(TARGETS Plugin_ParticleUniverse
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
