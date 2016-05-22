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

#ifndef __PU_VISUAL_PARTICLE_H__
#define __PU_VISUAL_PARTICLE_H__

#include "ParticleUniverseMath.h"
#include "ParticleUniverseParticle.h"

#include "OGRE/OgreColourValue.h"

namespace ParticleUniverse {

	/** A VisualParticle is the most obvious implementation of a particle. It represents that particles that can be
		visualised on the screen.
    */
	class _ParticleUniverseExport VisualParticle : public Particle {
	public:
		VisualParticle();
		virtual ~VisualParticle() {}

		/** Current and original colour */
		ColourValue colour;
		ColourValue originalColour;

		/** zRotation is used to rotate the particle in 2D (around the Z-axis)
		@remarks
			There is no relation between zRotation and orientation.
			rotationSpeed in combination with orientation are used for 3D rotation of the particle, while
			zRotation means the rotation around the Z-axis. This type of rotation is typically used for 
			rotating textures. This also means that both types of rotation can be used together.
		*/
		Radian zRotation;

		/** The zRotationSpeed is used in combination with zRotation and defines tha actual rotationspeed
			in 2D. */
		Radian zRotationSpeed;

		/*  Orientation of the particle.
		@remarks
			The orientation of the particle is only visible if the Particle Renderer - such as the Box renderer - 
			supports orientation.
		*/
		Quaternion orientation;
		Quaternion originalOrientation;

		/** The rotation is used in combination with orientation. Because the rotation speed is part
			of the particle itself, it can be changed independently. */
		Real rotationSpeed;

		/** The rotation axis is used in combination with orientation. Because the rotation axis is part
			of the particle itself, it can be changed independently. */
		Vector3 rotationAxis;

		/** Does this particle have it's own dimensions? */
		bool ownDimensions;

		/** Own width
		*/
		Real width;
        
		/** Own height
		*/
		Real height;

		/** Own depth
		*/
		Real depth;

		/** Radius of the particle, to be used for inter-particle collision and such.
		*/
		Real radius;

		/** Animation attributes
		*/
		Real textureAnimationTimeStep;
		Real textureAnimationTimeStepCount;
		uint16 textureCoordsCurrent;
		bool textureAnimationDirectionUp;

		/** Set own dimensions
		*/
		void setOwnDimensions(Real newWidth, Real newHeight, Real newDepth);

		/** @see Particle::_initForEmission()
		*/
		virtual void _initForEmission();

		/** @see Particle::_initForExpiration()
		*/
		virtual void _initForExpiration(ParticleTechnique * technique, Real timeElapsed);

		/** Calculate the bounding sphere radius
		*/
		void _calculateBoundingSphereRadius();
	};

} /* namespace ParticleUniverse */

#endif /* __PU_VISUAL_PARTICLE_H__ */
