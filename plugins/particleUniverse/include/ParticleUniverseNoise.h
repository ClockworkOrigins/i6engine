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

#ifndef __PU_NOISE_H__
#define __PU_NOISE_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	class _ParticleUniverseExport Noise3D {
	public:
		/* Constructor / Destructor */
		Noise3D();
		virtual ~Noise3D();

		/* Inititialises the noise function */
		void initialise(ushort octaves, double frequency = 1.0, double amplitude = 1.0, double persistence = 1.0);

		/* Returns a noise value between [0, 1]
		@remarks
			The noise is calculated in realtime
		*/
		double noise(double x, double y, double z);

		/* Returns a noise value between [0, 1]
		@remarks
			The noise is calculated in realtime
		*/
		double noise(const Vector3 & position);

		/* Creates an image file to test the noise */
		void noise2img(ushort dimension = 255);

	protected:
		int p[512];
		ushort mOctaves;
		double mFrequency;
		double mAmplitude;
		double mPersistence;

		/* Returns a noise value between [0, 1]
		@remarks
			The noise is calculated in realtime
		*/
		double _noise(double x, double y, double z);

		double _fade(double t);
		double _lerp(double t, double a, double b);
		double _grad(int hash, double x, double y, double z);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_NOISE_H__ */
