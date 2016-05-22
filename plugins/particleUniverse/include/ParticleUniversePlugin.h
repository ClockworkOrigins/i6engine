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

#ifndef __PARTICLE_UNIVERSE_PLUGIN_H__
#define __PARTICLE_UNIVERSE_PLUGIN_H__

#include "ParticleUniverseTypes.h"

#include "OGRE/OgrePlugin.h"

namespace ParticleUniverse {

	class ExternFactory;
	class ParticleAffectorFactory;
	class ParticleBehaviourFactory;
	class ParticleEmitterFactory;
	class ParticleEventHandlerFactory;
	class ParticleObserverFactory;
	class ParticleRendererFactory;
	class ParticleSystemManager;

	/** Plugin instance for Particle Universe */
	class ParticleUniversePlugin : public Ogre::Plugin {
	public:
		ParticleUniversePlugin();

		/// @copydoc Plugin::getName
		const String & getName() const;

		/// @copydoc Plugin::install
		void install();

		/// @copydoc Plugin::initialise
		void initialise();

		/// @copydoc Plugin::shutdown
		void shutdown();

		/// @copydoc Plugin::uninstall
		void uninstall();

	protected:
		ParticleSystemManager * mParticleSystemManager;
#ifdef PU_PHYSICS_PHYSX
		PhysXBridge* mPhysXBridge;
#endif // PU_PHYSICS_PHYSX

		vector<ParticleRendererFactory *> mRendererFactories;
		vector<ParticleEmitterFactory *> mEmitterFactories;
		vector<ParticleAffectorFactory *> mAffectorFactories;
		vector<ParticleObserverFactory *> mObserverFactories;
		vector<ParticleEventHandlerFactory *> mEventHandlerFactories;
		vector<ExternFactory *> mExternFactories;
		vector<ParticleBehaviourFactory *> mBehaviourFactories;
	};

} /* namespace ParticleUniverse */

#endif /* __PARTICLE_UNIVERSE_PLUGIN_H__ */
