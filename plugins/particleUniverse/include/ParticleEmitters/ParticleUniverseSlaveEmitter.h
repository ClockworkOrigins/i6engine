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

#ifndef __PU_SLAVE_EMITTER_H__
#define __PU_SLAVE_EMITTER_H__

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseTechniqueListener.h"

namespace ParticleUniverse {

	/** The SlaveEmitter makes the particle act as a slave, so it follows another particle to which it is related. This only applies
		at the creation (initialisation) of the particle. For further slave behaviour during the lifespan of the particle, the 
		SlaveBehaviour must be used also.
    */
	class _ParticleUniverseExport SlaveEmitter : public ParticleEmitter, public TechniqueListener {
	public:
		using Particle::copyAttributesTo;

		SlaveEmitter();
	    virtual ~SlaveEmitter() {}

		/** 
	    */
		const String & getMasterTechniqueName() const;

		/** 
	    */
		void setMasterTechniqueName(const String & masterTechniqueName);

		/** 
	    */
		const String & getMasterEmitterName() const;

		/** See ParticleEmitter.
	    */
		virtual void _initParticlePosition(Particle * particle);

		/** See ParticleEmitter.
	    */
		virtual void _initParticleDirection(Particle * particle);

		/** See ParticleEmitter.
	    */
		virtual void _prepare(ParticleTechnique * particleTechnique);

		/** See ParticleEmitter.
	    */
		virtual void _unprepare(ParticleTechnique * particleTechnique);

		/** See ParticleEmitter.
		*/
		virtual void _notifyStart();

		/** Initialise the emitted particle. This means that its position is set.
	    */
		virtual void particleEmitted(ParticleTechnique * particleTechnique, Particle * particle);

		/** No implementation.
	    */
		virtual void particleExpired(ParticleTechnique * particleTechnique, Particle * particle){}

		/** 
	    */
		void setMasterEmitterName(const String & masterEmitterName);

		/** 
	    */
		virtual void copyAttributesTo(ParticleEmitter * emitter);

	protected:
		Particle * mMasterParticle;
		String mMasterTechniqueName;
		String mMasterEmitterName;
		Vector3 mMasterPosition;
		Vector3 mMasterDirection;
		bool mMasterEmitterNameSet;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SLAVE_EMITTER_H__ */
