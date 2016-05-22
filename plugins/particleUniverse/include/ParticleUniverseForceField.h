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

#ifndef __PU_FORCE_FIELD_H__
#define __PU_FORCE_FIELD_H__

#include "ParticleUniverseNoise.h"

#include "OGRE/OgreWorkQueue.h"

namespace ParticleUniverse {

	/** Pure virtual ForceFieldCalculationFactory
	*/
	class _ParticleUniverseExport ForceFieldCalculationFactory : public FactoryAlloc {
	public:
		// Const
		static const Vector3 DEFAULT_WORLDSIZE;

		ForceFieldCalculationFactory() : mOctaves(2), mFrequency(1.0), mAmplitude(1.0), mPersistence(1.0), mMapScale(Vector3::ZERO), mWorldSize(DEFAULT_WORLDSIZE), mMappedPosition(Vector3::ZERO) {
		}
		virtual ~ForceFieldCalculationFactory() {}

		/** Generates the force field
		@remarks
			This function takes several arguments. 
			- forceFieldSize defines the internal dimensions of the force field. 
			- octaves, frequency, amplitude and persistence define the noise that is being generated.
			- worldSize defines the dimensions in the real world (scene).
		*/
		virtual void generate(unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize) = 0;

		/** Determine force, based on the position of a particle.
		#remarks
			The position is the position of a particle. The argument 'delta' defines the radius around the position that contributes to the 
			calculation of the force.
		*/
		virtual void determineForce(const Vector3 & position, Vector3 & force, Real delta) = 0;

		/** Default Getters/Setters
		*/
		virtual ushort getOctaves() const;
		virtual void setOctaves(ushort octaves);
		virtual double getFrequency() const;
		virtual void setFrequency(double frequency);
		virtual double getAmplitude() const;
		virtual void setAmplitude(double amplitude);
		virtual double getPersistence() const;
		virtual void setPersistence(double persistence);
		virtual unsigned int getForceFieldSize() const;
		virtual void setForceFieldSize(unsigned int forceFieldSize);
		virtual Vector3 getWorldSize() const;
		virtual void setWorldSize(const Vector3 & worldSize);

	protected:
		Noise3D mNoise3D;
		ushort mOctaves;
		double mFrequency;
		double mAmplitude;
		double mPersistence;
		Vector3 mMapScale;
		Vector3 mWorldSize;
		Vector3 mMappedPosition;
	};

	/** Factory class to calculate forces in realtime
	*/
	class _ParticleUniverseExport RealTimeForceFieldCalculationFactory : public ForceFieldCalculationFactory {
	public:
		RealTimeForceFieldCalculationFactory() : ForceFieldCalculationFactory() {}
		virtual ~RealTimeForceFieldCalculationFactory() {}

		/** Override from ForceFieldCalculationFactory
		*/
		virtual void generate(unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize);

		/** Override from ForceFieldCalculationFactory
		*/
		virtual void determineForce(const Vector3 & position, Vector3 & force, Real delta);
	};

	/** Factory class to calculate forces based on a precalculated force field
	*/
	class _ParticleUniverseExport MatrixForceFieldCalculationFactory : public ForceFieldCalculationFactory, public Ogre::WorkQueue::RequestHandler, public Ogre::WorkQueue::ResponseHandler {
	public:
		static const uint16 WORKQUEUE_GENERATE_REQUEST;
		struct MatrixPosition {
			Real x;
			Real y;
			Real z;
		};

		MatrixForceFieldCalculationFactory();
		virtual ~MatrixForceFieldCalculationFactory();

		/** Override from ForceFieldCalculationFactory
		*/
		virtual void generate(unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize);

		/** Delete the internally build matrix
		*/
		void deleteMatrix();

		/** Override from ForceFieldCalculationFactory
		*/
		virtual void determineForce(const Vector3 & position, Vector3 & force, Real delta);

		/** WorkQueue::RequestHandler override
		*/
		bool canHandleRequest(const Ogre::WorkQueue::Request * req, const Ogre::WorkQueue * srcQ);

		/** WorkQueue::RequestHandler override
		*/
		Ogre::WorkQueue::Response * handleRequest(const Ogre::WorkQueue::Request * req, const Ogre::WorkQueue * srcQ);

		/** WorkQueue::ResponseHandler override
		*/
		bool canHandleResponse(const Ogre::WorkQueue::Response * res, const Ogre::WorkQueue * srcQ);

		/** WorkQueue::ResponseHandler override
		*/
		void handleResponse(const Ogre::WorkQueue::Response * res, const Ogre::WorkQueue * srcQ);

		/** Get/Set Forcefield size.
		@remarks
			These get/set functions override default ones.
		*/
		virtual unsigned int getForceFieldSize() const;
		virtual void setForceFieldSize(unsigned int forceFieldSize);

	protected:
		MatrixPosition *** mMatrixPositions;
		unsigned int mForceFieldSize;
		unsigned int mX;
		unsigned int mY;
		unsigned int mZ;
		uint16 mWorkQueueChannel;
		bool mInitialising;

		// Request / response structures
		struct MatrixRequest {
			unsigned int forceFieldSize;
			ushort octaves;
			double frequency;
			double amplitude;
			double persistence;
			Vector3 worldSize;
			MatrixForceFieldCalculationFactory * src;
			_ParticleUniverseExport friend std::ostream & operator<<(std::ostream & o, const MatrixRequest & r) { return o; }
			MatrixRequest(MatrixForceFieldCalculationFactory * f): src(f) {}
		};
			
		struct MatrixResponse {
			_ParticleUniverseExport friend std::ostream & operator<<(std::ostream & o, const MatrixResponse & r) { return o; }
		};
	};

	/** Force Field Affector Class:
		This class defines a force field to affect the particle direction. The force field is based on 3D noise. The force can be calculated in
		realtime or based on a precreated 3D force field matrix, which essentially involves one lookup. To speed things up, the 3d matrix can be
		precreated in a separate thread (optionally).
	*/
	class _ParticleUniverseExport ForceField {
	public:
		enum ForceFieldType {
			FF_REALTIME_CALC,
			FF_MATRIX_CALC
		};

		ForceField();
		virtual ~ForceField();

		/** Initialises a ForceField */
		virtual void initialise(ForceFieldType type, const Vector3 & position, unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize);

		/** Initialises a ForceField */
		virtual void initialise(ForceFieldType type, unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize);

		/** Get/Set the base position of the force field */
		const Vector3 & getForceFieldPositionBase() const;
		void setForceFieldPositionBase(const Vector3 & position);

		/** Calculate the force, based on a certain position */
		void determineForce(const Vector3 & position, Vector3 & force, Real delta);

		/** Getters/Setters
		*/
		virtual ushort getOctaves() const;
		virtual void setOctaves(ushort octaves);
		virtual double getFrequency() const;
		virtual void setFrequency(double frequency);
		virtual double getAmplitude() const;
		virtual void setAmplitude(double amplitude);
		virtual double getPersistence() const;
		virtual void setPersistence(double persistence);
		virtual unsigned int getForceFieldSize() const;
		virtual void setForceFieldSize(unsigned int forceFieldSize);
		virtual Vector3 getWorldSize() const;
		virtual void setWorldSize(const Vector3 & worldSize);
			
		/** Get/Set the Forcefield type
		*/
		ForceFieldType getForceFieldType() const;
		void setForceFieldType(const ForceFieldType forceFieldType);

	protected:
		ushort mOctaves;
		double mFrequency;
		double mAmplitude;
		double mPersistence;
		Vector3 mWorldSize;
		unsigned int mForceFieldSize;
		ForceFieldCalculationFactory * mForceFieldCalculationFactory; // Internal factory that creates a certain force field type
		Vector3 mForceFieldPositionBase; // Position of the force field
		ForceFieldType mForceFieldType;

		/** Get/Set/Create ForceFieldCalculationFactory */
		ForceFieldCalculationFactory * getForceFieldCalculationFactory() const;
		void setForceFieldCalculationFactory(ForceFieldCalculationFactory * forceFieldCalculationFactory);
		ForceFieldCalculationFactory * createForceFieldCalculationFactory(ForceFieldType type);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_FORCE_FIELD_H__ */
