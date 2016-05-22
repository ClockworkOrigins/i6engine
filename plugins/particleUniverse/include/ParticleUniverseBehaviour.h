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

#ifndef __PU_BEHAVIOUR_H__
#define __PU_BEHAVIOUR_H__

#include "ParticleUniverseIAlias.h"
#include "ParticleUniverseIElement.h"

#include "OGRE/OgreVector3.h"

namespace ParticleUniverse {

	/** Defines the behaviour of a particle.
	@remarks
		While a ParticleAffector acts as an external 'force' on the particles, the ParticleBehaviour defines 
		the internal behaviour of each particle individually. For example, 'wind' is typically something that is 
		defined as a ParticleAffector, while 'elasticity' is behaviour of the particle itself.
	@par
		A particle can have multiple Behaviours. Each ParticleBehaviour is accompanied with a specific subclass of 
		the ParticleBehaviourFactory. Similar to ParticleEmitters and ParticleAffectors, each factory is 
		identified by means of a type name.
	@par
		The main reason to introduce the ParticleBehaviour class is to be able to expand the attributes and 
		functions of a particle dynamically and to assign behaviour by means of a particle script.
	*/
	class _ParticleUniverseExport ParticleBehaviour : public IAlias, public IElement {
	public:
		ParticleBehaviour() : IAlias(), IElement(), mParentTechnique(nullptr), _mBehaviourScale(Vector3::UNIT_SCALE) {
			mAliasType = AT_BEHAVIOUR;
		}
		virtual ~ParticleBehaviour() {}

		/** Todo
	    */
		const String & getBehaviourType() const { return mBehaviourType; }
		void setBehaviourType(const String & behaviourType) { mBehaviourType = behaviourType; }

		/** Todo
	    */
		ParticleTechnique * getParentTechnique() const { return mParentTechnique; }
		void setParentTechnique(ParticleTechnique * parentTechnique) { mParentTechnique = parentTechnique; }

		/** Notify that the Behaviour is rescaled.
	    */
		virtual void _notifyRescaled(const Vector3 & scale) { _mBehaviourScale = scale; }

		/** Perform initialisation actions.
		@remarks
			The _prepare() function is automatically called during initialisation (prepare) activities of a 
			ParticleTechnique. A subclass could implement this function to perform initialisation 
			actions.
	    */
		virtual void _prepare(ParticleTechnique * technique) { /* No implementation */ }

		/** Reverse the actions from the _prepare.
	    */
		virtual void _unprepare(ParticleTechnique * particleTechnique) { /* No implementation */ }

		/** Perform initialising activities as soon as the particle with which the ParticleBehaviour is
			associated, is emitted.
	    */
		virtual void _initParticleForEmission(Particle * particle) { /* No implementation */ }

		/** Process a particle.
	    */
		virtual void _processParticle(ParticleTechnique * technique, Particle * particle, Real timeElapsed) = 0;

		/** Perform some action if a particle expires.
	    */
		virtual void _initParticleForExpiration(ParticleTechnique * technique, Particle * particle, Real timeElapsed) { /* No implementation */ }

		/** Copy attributes to another ParticleBehaviour.
	    */
		virtual void copyAttributesTo(ParticleBehaviour * behaviour) {
			copyParentAttributesTo(behaviour);
		}

		/** Copy parent attributes to another behaviour.
	    */
		virtual void copyParentAttributesTo(ParticleBehaviour * behaviour) {
			behaviour->mParentTechnique = mParentTechnique;
			behaviour->_mBehaviourScale = _mBehaviourScale;
		}

	protected:
		// Type of behaviour
		String mBehaviourType;

		ParticleTechnique * mParentTechnique;

		/** Although the scale is on a Particle System level, the behaviour can also be scaled.
		*/
		Vector3 _mBehaviourScale;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BEHAVIOUR_H__ */
