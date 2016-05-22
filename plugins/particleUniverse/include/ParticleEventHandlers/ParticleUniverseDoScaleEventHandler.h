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

#ifndef __PU_DO_SCALE_EVENT_HANDLER_H__
#define __PU_DO_SCALE_EVENT_HANDLER_H__

#include "ParticleUniverseEventHandler.h"

namespace ParticleUniverse {

	/** The DoScaleEventHandler scales different particle attributes.
    */
	class _ParticleUniverseExport DoScaleEventHandler : public ParticleEventHandler {
	public:
		enum ScaleType {
			ST_TIME_TO_LIVE,
			ST_VELOCITY
		};

		// Constants
		static const ScaleType DEFAULT_SCALE_TYPE;
		static const Real DEFAULT_SCALE_FRACTION;

		DoScaleEventHandler();
	    virtual ~DoScaleEventHandler() {}

		/** Returns the scale type
	    */
		const ScaleType & getScaleType() const;

		/** Set the scale type. This scale type identifies to which attribute the scale factor is applied.
	    */
		void setScaleType(const ScaleType & scaleType);

		/** Returns the scale fraction
	    */
		Real getScaleFraction() const;

		/** Set the scale fraction. This scale fraction value is used to scale different attributes if the 
			event handler is called.
	    */
		void setScaleFraction(const Real scaleFraction);

		/** 
	    */
		virtual void _handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** Copy attributes to another event handler.
	    */
		virtual void copyAttributesTo(ParticleEventHandler * eventHandler);

	protected:
		Real mScaleFraction;
		ScaleType mScaleType;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_DO_SCALE_EVENT_HANDLER_H__ */
