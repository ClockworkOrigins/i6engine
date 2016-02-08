/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
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
