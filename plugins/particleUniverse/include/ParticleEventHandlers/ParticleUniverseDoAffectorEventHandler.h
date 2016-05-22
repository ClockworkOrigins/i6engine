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

#ifndef __PU_DO_AFFECTOR_EVENT_HANDLER_H__
#define __PU_DO_AFFECTOR_EVENT_HANDLER_H__

#include "ParticleUniverseEventHandler.h"

namespace ParticleUniverse {

	/** This class explicitly calls an affector to affect the particles. There are several reasons why
		this is appropriate. One reason is, that you only want to affect a particle if a certain event
		occurs. Disable an affector and call it using this event handler, is the method (calling the affector
		from the event handler doesn't take into consideration that the affector is disabled).
    */
	class _ParticleUniverseExport DoAffectorEventHandler : public ParticleEventHandler {
	public:
		// Constants
		static const bool DEFAULT_PRE_POST;

		DoAffectorEventHandler();
	    virtual ~DoAffectorEventHandler() {}

		/** Get the indication whether pre- and postprocessing must be done.
	    */
		bool getPrePost() const { return mPrePost; }

		/** Set the indication whether pre- and postprocessing must be done.
	    */
		void setPrePost(const bool prePost) { mPrePost = prePost; }

		/** Get the name of the affector that must be enabled or disabled.
	    */
		const String & getAffectorName() const { return mAffectorName; }

		/** Set the name of the affector.
	    */
		void setAffectorName(const String & affectorName) { mAffectorName = affectorName; }

		/** If the _handle() function of this class is invoked (by an Observer), it searches the 
			ParticleAffector defined by the its name. 
			The ParticleAffector is either part of the ParticleTechnique in which the 
			DoAffectorEventHandler is defined, or if the Affector is not found, other 
			ParticleTechniques are searched.
	    */
		virtual void _handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** Copy attributes to another event handler.
	    */
		virtual void copyAttributesTo(ParticleEventHandler * eventHandler);

	protected:
		// Identifies the name of affector
		String mAffectorName;

		// Determines whether the pre- and post processing activities must be executed also
		bool mPrePost;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_DO_AFFECTOR_EVENT_HANDLER_H__ */
