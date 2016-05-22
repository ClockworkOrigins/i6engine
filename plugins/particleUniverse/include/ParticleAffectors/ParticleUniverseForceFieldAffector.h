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

#ifndef __PU_FORCE_FIELD_AFFECTOR_H__
#define __PU_FORCE_FIELD_AFFECTOR_H__

#include "ParticleUniverseAffector.h"
#include "ParticleUniverseForceField.h"

namespace ParticleUniverse {

	/** Force Field Affector Class:
		This Affector Class uses a force field to affect the particle direction.
	*/
	class _ParticleUniverseExport ForceFieldAffector : public ParticleAffector {
	public:
		using Particle::copyAttributesTo;

		// Constants
		static const ForceField::ForceFieldType DEFAULT_FORCEFIELD_TYPE;
		static const Real DEFAULT_DELTA;
		static const Real DEFAULT_FORCE;
		static const ushort DEFAULT_OCTAVES;
		static const double DEFAULT_FREQUENCY;
		static const double DEFAULT_AMPLITUDE;
		static const double DEFAULT_PERSISTENCE;
		static const unsigned int DEFAULT_FORCEFIELDSIZE;
		static const Vector3 DEFAULT_WORLDSIZE;
		static const Vector3 DEFAULT_MOVEMENT;
		static const Real DEFAULT_MOVEMENT_FREQUENCY;

		ForceFieldAffector();
		~ForceFieldAffector();

		/** @copydoc ParticleAffector::_prepare */
		virtual void _prepare(ParticleTechnique * particleTechnique);

		/** @copydoc ParticleAffector::_preProcessParticles */
		virtual void _preProcessParticles(ParticleTechnique * particleTechnique, Real timeElapsed);

		/** @copydoc ParticleAffector::_notifyStart */
		virtual void _notifyStart();

		/** @copydoc ParticleAffector::_affect */
		virtual void _affect(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed);

		/** Get/Set Forcefield type
		*/
		ForceField::ForceFieldType getForceFieldType() const;
		void setForceFieldType(const ForceField::ForceFieldType forceFieldType);

		/** Get/Set Delta
		*/
		Real getDelta() const;
		void setDelta(Real delta);

		/** Get/Set scale Force
		*/
		Real getScaleForce() const;
		void setScaleForce(Real scaleForce);

		/** Get/Set scale Octaves
		*/
		ushort getOctaves() const;
		void setOctaves(ushort octaves);

		/** Get/Set scale Frequency
		*/
		double getFrequency() const;
		void setFrequency(double frequency);

		/** Get/Set scale Amplitude
		*/
		double getAmplitude() const;
		void setAmplitude(double amplitude);

		/** Get/Set scale Persistence
		*/
		double getPersistence() const;
		void setPersistence(double persistence);

		/** Get/Set scale ForceFieldSize
		*/
		unsigned int getForceFieldSize() const;
		void setForceFieldSize(unsigned int forceFieldSize);

		/** Get/Set scale worldSize
		*/
		Vector3 getWorldSize() const;
		void setWorldSize(const Vector3 & worldSize);

		/** Get/Set scale flip attributes
		*/
		bool getIgnoreNegativeX() const;
		void setIgnoreNegativeX(bool ignoreNegativeX);
		bool getIgnoreNegativeY() const;
		void setIgnoreNegativeY(bool ignoreNegativeY);
		bool getIgnoreNegativeZ() const;
		void setIgnoreNegativeZ(bool ignoreNegativeZ);

		/** Get/Set Movement
		@remarks
			The movement vector determines the position of the movement. This movement is a displacement of the particle position 
			mapped to the forcefield.
		*/
		const Vector3 & getMovement() const;
		void setMovement(const Vector3 & movement);

		/** Get/Set Movement frequency
		*/
		Real getMovementFrequency() const;
		void setMovementFrequency(Real movementFrequency);

		/** Suppress (re)generation of the forcefield everytime an attribute is changed.
		*/
		void suppressGeneration(bool suppress);

		/** @copydoc ParticleAffector::copyAttributesTo */
		virtual void copyAttributesTo(ParticleAffector * affector);

	protected:
		ForceField mForceField; // Local force field
		ForceField::ForceFieldType mForceFieldType; // Type of force field
		Real mDelta; // Radius of particle position
		Vector3 mForce; // Force value that is used to calculate the force and reused for all particles
		Real mScaleForce; // Scaling factor used in calculation of the direction vector of the particle
		ushort mOctaves; // Used in noise generation
		double mFrequency; // Used in noise generation
		double mAmplitude; // Used in noise generation
		double mPersistence; // Used in noise generation
		unsigned int mForceFieldSize; // The real size of the force field (is a unit cube in case of a 'realtime' force field type
		Vector3 mWorldSize; // Size of the force field in world coordinates (can be non-cubic)
		bool mPrepared; // Determines whether the force field is generated
		bool mIgnoreNegativeX; // X-force is never negative
		bool mIgnoreNegativeY; // Y-force is never negative
		bool mIgnoreNegativeZ; // Z-force is never negative
		Vector3 mMovement; // The direction to which the forcefield moves
		bool mMovementSet; // Is true if the movement is set
		Real mMovementFrequency; // Speed of movement
		Real mMovementFrequencyCount;
		Vector3 mDisplacement;
		Vector3 mBasePosition;
		bool mSuppressGeneration; // Prevents re-generation of the complete forcefield after a change.
	};

} /* namespace ParticleUniverse */

#endif /* __PU_FORCE_FIELD_AFFECTOR_H__ */
