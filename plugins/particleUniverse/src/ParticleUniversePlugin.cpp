/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ParticleUniversePlugin.h"

#include "ParticleUniverseAlloc.h"
#include "ParticleUniverseSystemManager.h"

#include "ParticleAffectors/ParticleUniverseAlignAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseBaseColliderFactory.h"
#include "ParticleAffectors/ParticleUniverseBaseForceAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseBoxColliderFactory.h"
#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseColourAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseFlockCenteringAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseForceFieldAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseGeometryRotatorFactory.h"
#include "ParticleAffectors/ParticleUniverseGravityAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseInterParticleColliderFactory.h"
#include "ParticleAffectors/ParticleUniverseJetAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseLineAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseLinearForceAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseParticleFollowerFactory.h"
#include "ParticleAffectors/ParticleUniversePathFollowerFactory.h"
#include "ParticleAffectors/ParticleUniversePlaneColliderFactory.h"
#include "ParticleAffectors/ParticleUniverseRandomiserFactory.h"
#include "ParticleAffectors/ParticleUniverseScaleAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseScaleVelocityAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseSineForceAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseSphereColliderFactory.h"
#include "ParticleAffectors/ParticleUniverseTextureAnimatorFactory.h"
#include "ParticleAffectors/ParticleUniverseTextureRotatorFactory.h"
#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffectorFactory.h"
#include "ParticleAffectors/ParticleUniverseVortexAffectorFactory.h"

#include "ParticleBehaviours/ParticleUniverseSlaveBehaviourFactory.h"

#include "ParticleEmitters/ParticleUniverseBoxEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseCircleEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseLineEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseMeshSurfaceEmitterFactory.h"
#include "ParticleEmitters/ParticleUniversePointEmitterFactory.h"
#include "ParticleEmitters/ParticleUniversePositionEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseSlaveEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitterFactory.h"
#include "ParticleEmitters/ParticleUniverseVertexEmitterFactory.h"

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoExpireEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoFreezeEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoPlacementParticleEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandlerFactory.h"
#include "ParticleEventHandlers/ParticleUniverseDoStopSystemEventHandlerFactory.h"

#include "Externs/ParticleUniverseBoxColliderExternFactory.h"
#include "Externs/ParticleUniverseGravityExternFactory.h"
#include "Externs/ParticleUniverseSceneDecoratorExternFactory.h"
#include "Externs/ParticleUniverseSphereColliderExternFactory.h"
#include "Externs/ParticleUniverseVortexExternFactory.h"

#include "ParticleObservers/ParticleUniverseOnClearObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnCollisionObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnCountObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnEmissionObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnEventFlagObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnExpireObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnPositionObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnQuotaObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnRandomObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnTimeObserverFactory.h"
#include "ParticleObservers/ParticleUniverseOnVelocityObserverFactory.h"

#include "ParticleRenderers/ParticleUniverseBeamRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseBillboardRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseBoxRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseEntityRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseLightRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseRibbonTrailRendererFactory.h"
#include "ParticleRenderers/ParticleUniverseSphereRendererFactory.h"

#include "OGRE/OgreRoot.h"

namespace ParticleUniverse {

	const String sPluginName = "ParticleUniverse";
	
	ParticleUniversePlugin::ParticleUniversePlugin() {
	}
	
	const String & ParticleUniversePlugin::getName() const {
		return sPluginName;
	}
	
	void ParticleUniversePlugin::install() {
#ifdef PU_PHYSICS_PHYSX
		// ----------------------- Create the PhysXBridge -----------------------
		mPhysXBridge = PU_NEW_T(PhysXBridge, MEMCATEGORY_SCENE_CONTROL)();
#endif // PU_PHYSICS_PHYSX

		// ----------------------- Create the particle system manager -----------------------
		mParticleSystemManager = PU_NEW_T(ParticleSystemManager, MEMCATEGORY_SCENE_CONTROL)();

		// ----------------------- Create renderer factories -----------------------
		ParticleRendererFactory * particleRendererFactory;

		particleRendererFactory = PU_NEW BeamRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW BillboardRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW BoxRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW SphereRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW EntityRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW RibbonTrailRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		particleRendererFactory = PU_NEW LightRendererFactory();
		mParticleSystemManager->addRendererFactory(particleRendererFactory);
		mRendererFactories.push_back(particleRendererFactory);

		// ----------------------- Create emitter factories -----------------------
		ParticleEmitterFactory * particleEmitterFactory;
		particleEmitterFactory = PU_NEW PointEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW LineEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW BoxEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW CircleEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW SphereSurfaceEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW VertexEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);
		
		particleEmitterFactory = PU_NEW MeshSurfaceEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW PositionEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		particleEmitterFactory = PU_NEW SlaveEmitterFactory();
		mParticleSystemManager->addEmitterFactory(particleEmitterFactory);
		mEmitterFactories.push_back(particleEmitterFactory);

		// ----------------------- Create affector factories -----------------------
		ParticleAffectorFactory * particleAffectorFactory;
		particleAffectorFactory = PU_NEW BaseForceAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW LinearForceAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW GravityAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW ParticleFollowerFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW VortexAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW RandomiserFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW LineAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW ScaleAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW ScaleVelocityAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW GeometryRotatorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW TextureRotatorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW TextureAnimatorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW JetAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW AlignAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW FlockCenteringAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW CollisionAvoidanceAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW VelocityMatchingAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW ColourAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW SineForceAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW BaseColliderFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW SphereColliderFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW PlaneColliderFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW BoxColliderFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW InterParticleColliderFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW PathFollowerFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		particleAffectorFactory = PU_NEW ForceFieldAffectorFactory();
		mParticleSystemManager->addAffectorFactory(particleAffectorFactory);
		mAffectorFactories.push_back(particleAffectorFactory);

		// ----------------------- Create observer factories -----------------------
		ParticleObserverFactory * particleObserverFactory;
		particleObserverFactory = PU_NEW OnExpireObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnEmissionObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnCountObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnEventFlagObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnCollisionObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnVelocityObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnTimeObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnPositionObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnClearObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnQuotaObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		particleObserverFactory = PU_NEW OnRandomObserverFactory();
		mParticleSystemManager->addObserverFactory(particleObserverFactory);
		mObserverFactories.push_back(particleObserverFactory);

		// ----------------------- Create event handler factories -----------------------
		ParticleEventHandlerFactory * particleEventHandlerFactory;
		particleEventHandlerFactory = PU_NEW DoExpireEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoFreezeEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoPlacementParticleEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoStopSystemEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoEnableComponentEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoAffectorEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		particleEventHandlerFactory = PU_NEW DoScaleEventHandlerFactory();
		mParticleSystemManager->addEventHandlerFactory(particleEventHandlerFactory);
		mEventHandlerFactories.push_back(particleEventHandlerFactory);

		// ----------------------- Create extern factories -----------------------
		ExternFactory * externFactory;
		externFactory = PU_NEW GravityExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

		externFactory = PU_NEW SphereColliderExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

		externFactory = PU_NEW BoxColliderExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

		externFactory = PU_NEW VortexExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

		externFactory = PU_NEW SceneDecoratorExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

#ifdef PU_PHYSICS_PHYSX
		externFactory = PU_NEW PhysXActorExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);

		externFactory = PU_NEW PhysXFluidExternFactory();
		mParticleSystemManager->addExternFactory(externFactory);
		mExternFactories.push_back(externFactory);
#endif //PU_PHYSICS_PHYSX

		// ----------------------- Create behaviour factories -----------------------
		ParticleBehaviourFactory * particleBehaviourFactory;
		particleBehaviourFactory = PU_NEW SlaveBehaviourFactory();
		mParticleSystemManager->addBehaviourFactory(particleBehaviourFactory);
		mBehaviourFactories.push_back(particleBehaviourFactory);
	}
	
	void ParticleUniversePlugin::initialise() {
	}
	
	void ParticleUniversePlugin::shutdown() {
		// Delete the manager.
		// The manager must be deleted before all factories are deleted (also the factories of other plugins)
		// PU_DELETE_T(mParticleSystemManager, ParticleSystemManager, MEMCATEGORY_SCENE_CONTROL); // TODO: (Daniel) check this, crashes actually
	}
	
	void ParticleUniversePlugin::uninstall() {
		// Delete all factories created in dllStartPlugin (after deleting the ParticleSystemManager)
		for (vector<ParticleRendererFactory *>::iterator ri = mRendererFactories.begin(); ri != mRendererFactories.end(); ++ri) {
			PU_DELETE *ri;
		}

		for (vector<ParticleEmitterFactory *>::iterator ei = mEmitterFactories.begin(); ei != mEmitterFactories.end(); ++ei) {
			PU_DELETE *ei;
		}

		for (vector<ParticleAffectorFactory *>::iterator ai = mAffectorFactories.begin(); ai != mAffectorFactories.end(); ++ai) {
			PU_DELETE *ai;
		}
		
		for (vector<ParticleObserverFactory *>::iterator oi = mObserverFactories.begin(); oi != mObserverFactories.end(); ++oi) {
			PU_DELETE *oi;
		}
		
		for (vector<ParticleEventHandlerFactory *>::iterator ehi = mEventHandlerFactories.begin(); ehi != mEventHandlerFactories.end(); ++ehi) {
			PU_DELETE *ehi;
		}

		for (vector<ExternFactory *>::iterator exi = mExternFactories.begin(); exi != mExternFactories.end(); ++exi) {
			PU_DELETE *exi;
		}

		for (vector<ParticleBehaviourFactory *>::iterator bi = mBehaviourFactories.begin(); bi != mBehaviourFactories.end(); ++bi) {
			PU_DELETE *bi;
		}

#ifdef PU_PHYSICS_PHYSX
		// Delete the PhysXBridge
		PU_DELETE_T(mPhysXBridge, PhysXBridge, MEMCATEGORY_SCENE_CONTROL);
#endif // PU_PHYSICS_PHYSX
	}

} /* namespace ParticleUniverse */
