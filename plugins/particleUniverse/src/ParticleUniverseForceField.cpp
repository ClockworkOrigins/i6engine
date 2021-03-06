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

#include "ParticleUniverseForceField.h"

#include "ParticleUniverseAny.h"

#include "OGRE/OgreRoot.h"

namespace ParticleUniverse {

	const Vector3 ForceFieldCalculationFactory::DEFAULT_WORLDSIZE = Vector3(500.0, 500.0, 500.0);
	
	ushort ForceFieldCalculationFactory::getOctaves() const {
		return mOctaves;
	}
	
	void ForceFieldCalculationFactory::setOctaves(ushort octaves) {
		mOctaves = octaves;
		generate(getForceFieldSize(), mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	double ForceFieldCalculationFactory::getFrequency() const {
		return mFrequency;
	}
	
	void ForceFieldCalculationFactory::setFrequency(double frequency) {
		mFrequency = frequency;
		generate(getForceFieldSize(), mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	double ForceFieldCalculationFactory::getAmplitude() const {
		return mAmplitude;
	}
	
	void ForceFieldCalculationFactory::setAmplitude(double amplitude) {
		mAmplitude = amplitude;
		generate(getForceFieldSize(), mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	double ForceFieldCalculationFactory::getPersistence() const {
		return mPersistence;
	}
	
	void ForceFieldCalculationFactory::setPersistence(double persistence) {
		mPersistence = persistence;
		generate(getForceFieldSize(), mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	unsigned int ForceFieldCalculationFactory::getForceFieldSize() const {
		return 1; // Return default cubic size
	}
	
	void ForceFieldCalculationFactory::setForceFieldSize(unsigned int forceFieldSize) {
		// The forcefield cannot be zero
		if (forceFieldSize == 0) {
			return;
		}

		generate(forceFieldSize, mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	Vector3 ForceFieldCalculationFactory::getWorldSize() const {
		return mWorldSize;
	}
	
	void ForceFieldCalculationFactory::setWorldSize(const Vector3 & worldSize) {
		// The worldsize cannot be zero
		if (worldSize == Vector3::ZERO) {
			return;
		}

		mWorldSize = worldSize;
		generate(getForceFieldSize(), mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	void RealTimeForceFieldCalculationFactory::generate(unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize) {
		mOctaves = octaves;
		mFrequency = frequency;
		mAmplitude = amplitude;
		mPersistence = persistence;
		mWorldSize = worldSize;
		mNoise3D.initialise(octaves, frequency, amplitude, persistence);

		if (worldSize != Vector3::ZERO) {
			mMapScale.x = 1.0 / worldSize.x; // Remark: forceFieldSize is not used, because it is a unit cube
			mMapScale.y = 1.0 / worldSize.y;
			mMapScale.z = 1.0 / worldSize.z;
		}
	}
	
	void RealTimeForceFieldCalculationFactory::determineForce(const Vector3 & position, Vector3 & force, Real delta) {
		mMappedPosition.x = mMapScale.x * position.x;
		mMappedPosition.y = mMapScale.y * position.y;
		mMappedPosition.z = mMapScale.z * position.z;

		if (mMappedPosition.x < 0.0 || mMappedPosition.x > 1.0 || mMappedPosition.y < 0.0 || mMappedPosition.y > 1.0 || mMappedPosition.z < 0.0 || mMappedPosition.z > 1.0) {
			// Position is outside the forcefield (outside the unit cube)
			return;
		}

		force.x = Real(mNoise3D.noise(mMappedPosition.x + delta, mMappedPosition.y, mMappedPosition.z) - mNoise3D.noise(mMappedPosition.x - delta, mMappedPosition.y, mMappedPosition.z));
		force.y = Real(mNoise3D.noise(mMappedPosition.x, mMappedPosition.y + delta, mMappedPosition.z) - mNoise3D.noise(mMappedPosition.x, mMappedPosition.y - delta, mMappedPosition.z));
		force.z = Real(mNoise3D.noise(mMappedPosition.x, mMappedPosition.y, mMappedPosition.z + delta) - mNoise3D.noise(mMappedPosition.x, mMappedPosition.y, mMappedPosition.z - delta));
	}
	
	const uint16 MatrixForceFieldCalculationFactory::WORKQUEUE_GENERATE_REQUEST = 1;
	
	MatrixForceFieldCalculationFactory::MatrixForceFieldCalculationFactory() : ForceFieldCalculationFactory(), mMatrixPositions(nullptr), mForceFieldSize(0), mX(0), mY(0), mZ(0), mInitialising(false) {
		Ogre::WorkQueue * wq = Ogre::Root::getSingleton().getWorkQueue();
		mWorkQueueChannel = wq->getChannel("ParticleUniverse/MatrixForceFieldCalculationFactory");
		wq->addRequestHandler(mWorkQueueChannel, this);
		wq->addResponseHandler(mWorkQueueChannel, this);
	}
	
	MatrixForceFieldCalculationFactory::~MatrixForceFieldCalculationFactory() {
		Ogre::WorkQueue * wq = Ogre::Root::getSingleton().getWorkQueue();
		wq->removeRequestHandler(mWorkQueueChannel, this);
		wq->removeResponseHandler(mWorkQueueChannel, this);
		deleteMatrix();
	}
	
	void MatrixForceFieldCalculationFactory::generate(unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize) {
		// Push request to the workerqueue
		MatrixRequest req(this);
		req.forceFieldSize = forceFieldSize;
		req.octaves = octaves;
		req.frequency = frequency;
		req.amplitude = amplitude;
		req.persistence = persistence;
		req.worldSize = worldSize;
		mInitialising = true;
		Ogre::Root::getSingleton().getWorkQueue()->addRequest(mWorkQueueChannel, WORKQUEUE_GENERATE_REQUEST, Any(req), 0, false);
	}
	
	void MatrixForceFieldCalculationFactory::deleteMatrix() {
		if (!mMatrixPositions) {
			return;
		}

		for (unsigned int i = 0; i < mForceFieldSize; ++i) {
			for (unsigned int j = 0; j < mForceFieldSize; ++j) {
				delete[] mMatrixPositions[i][j];
			}
			delete[] mMatrixPositions[i];
		}
		delete[] mMatrixPositions;
		mMatrixPositions = nullptr;
	}
	
	void MatrixForceFieldCalculationFactory::determineForce(const Vector3 & position, Vector3 & force, Real delta) {
		if (mInitialising) {
			return;
		}

		// Ignore delta, because it is not needed to search into the matrix
		mX = static_cast<unsigned int>(mMapScale.x * position.x);
		mY = static_cast<unsigned int>(mMapScale.y * position.y);
		mZ = static_cast<unsigned int>(mMapScale.z * position.z);
		if (mX >= mForceFieldSize || mY >= mForceFieldSize || mZ >= mForceFieldSize) {
			// Position is outside the forcefield
			return;
		}

		force.x = mMatrixPositions[mX][mY][mZ].x;
		force.y = mMatrixPositions[mX][mY][mZ].y;
		force.z = mMatrixPositions[mX][mY][mZ].z;
	}
	
	bool MatrixForceFieldCalculationFactory::canHandleRequest(const Ogre::WorkQueue::Request * req, const Ogre::WorkQueue * srcQ) {
		MatrixRequest preq = any_cast<MatrixRequest>(req->getData());
		if (preq.src != this) {
			return false;
		} else {
			return RequestHandler::canHandleRequest(req, srcQ);
		}
	}
	
	bool MatrixForceFieldCalculationFactory::canHandleResponse(const Ogre::WorkQueue::Response * res, const Ogre::WorkQueue * srcQ) {
		MatrixRequest preq = any_cast<MatrixRequest>(res->getRequest()->getData());
		if (preq.src != this) {
			return false;
		} else {
			return true;
		}
	}
	
	Ogre::WorkQueue::Response * MatrixForceFieldCalculationFactory::handleRequest(const Ogre::WorkQueue::Request * req, const Ogre::WorkQueue * srcQ) {
		MatrixRequest preq = any_cast<MatrixRequest>(req->getData());
		if (preq.src != this) {
			return nullptr;
		}

		// Setup the 3D matrix with precalculated force field vectors
		mNoise3D.initialise(preq.octaves, preq.frequency, preq.amplitude, preq.persistence);
		mMapScale.x = preq.forceFieldSize / preq.worldSize.x;
		mMapScale.y = preq.forceFieldSize / preq.worldSize.y;
		mMapScale.z = preq.forceFieldSize / preq.worldSize.z;

		deleteMatrix();
		mForceFieldSize = preq.forceFieldSize;
		mMatrixPositions = new MatrixPosition**[mForceFieldSize];
		for (unsigned int i = 0; i < mForceFieldSize; ++i) {
			mMatrixPositions[i] = new MatrixPosition*[mForceFieldSize];
			for (unsigned int j = 0; j < mForceFieldSize; ++j) {
				mMatrixPositions[i][j] = new MatrixPosition[mForceFieldSize];
			}
		}
	
		// Fill the matrix
		mForceFieldSize = mForceFieldSize == 0 ? 1 : mForceFieldSize; // The forcefield may not be 0
		Real delta = 1.0 / mForceFieldSize;
		for (unsigned int i = 0; i < mForceFieldSize; ++i) {
			for (unsigned int j = 0; j < mForceFieldSize; ++j) {
				for (unsigned int k = 0; k < mForceFieldSize; ++k) {
					mMatrixPositions[i][j][k].x = Real(mNoise3D.noise(i * delta + delta, j * delta, k * delta) - mNoise3D.noise(i * delta - delta, j * delta, k * delta));
					mMatrixPositions[i][j][k].y = Real(mNoise3D.noise(i * delta, j * delta + delta, k * delta) - mNoise3D.noise(i * delta, j * delta - delta, k * delta));
					mMatrixPositions[i][j][k].z = Real(mNoise3D.noise(i * delta, j * delta, k * delta + delta) - mNoise3D.noise(i * delta, j * delta, k * delta - delta));
				}
			}
		}

		MatrixResponse res;
		Ogre::WorkQueue::Response * response = PU_NEW Ogre::WorkQueue::Response(req, true, Any(res));
		return response;
	}
	
	void MatrixForceFieldCalculationFactory::handleResponse(const Ogre::WorkQueue::Response * res, const Ogre::WorkQueue * srcQ) {
		// Response itself is not used
		mInitialising = false;
	}
	
	unsigned int MatrixForceFieldCalculationFactory::getForceFieldSize() const {
		return mForceFieldSize;
	}
	
	void MatrixForceFieldCalculationFactory::setForceFieldSize(unsigned int forceFieldSize) {
		mForceFieldSize = forceFieldSize;
		generate(mForceFieldSize, mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
	}
	
	ForceField::ForceField() : mOctaves(2), mFrequency(1.0), mAmplitude(1.0), mPersistence(1.0), mWorldSize(ForceFieldCalculationFactory::DEFAULT_WORLDSIZE), mForceFieldSize(64), mForceFieldCalculationFactory(nullptr), mForceFieldPositionBase(Vector3::ZERO), mForceFieldType(FF_REALTIME_CALC) {
	}
	
	ForceField::~ForceField() {
		if (mForceFieldCalculationFactory) {
			PU_DELETE mForceFieldCalculationFactory;
		}
	}
	
	void ForceField::initialise(ForceFieldType type, const Vector3 & position, unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize) {
		// Initialise first
		initialise(type, forceFieldSize, octaves, frequency, amplitude, persistence, worldSize);

		// Store the base and max position of the forcefield.
		mForceFieldPositionBase = position;
		mForceFieldPositionBase.x -= 0.5 * worldSize.x;
		mForceFieldPositionBase.y -= 0.5 * worldSize.y;
		mForceFieldPositionBase.z -= 0.5 * worldSize.z;
	}
	
	void ForceField::initialise(ForceFieldType type, unsigned int forceFieldSize, ushort octaves, double frequency, double amplitude, double persistence, const Vector3 & worldSize) {
		// Create a factory
		mForceFieldCalculationFactory = createForceFieldCalculationFactory(type);

		// Create all necessary data for the force field
		mForceFieldCalculationFactory->generate(forceFieldSize, octaves, frequency, amplitude, persistence, worldSize);
	}
	
	const Vector3 & ForceField::getForceFieldPositionBase() const {
		return mForceFieldPositionBase;
	}
	
	void ForceField::setForceFieldPositionBase(const Vector3 & position) {
		mForceFieldPositionBase = position;
	}
	
	void ForceField::determineForce(const Vector3 & position, Vector3 & force, Real delta) {
		force.x = 0.0;
		force.y = 0.0;
		force.z = 0.0;

		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->determineForce(position - mForceFieldPositionBase, force, delta);
		}
	}
	
	ForceFieldCalculationFactory * ForceField::getForceFieldCalculationFactory() const {
		return mForceFieldCalculationFactory;
	}
	
	void ForceField::setForceFieldCalculationFactory(ForceFieldCalculationFactory * forceFieldCalculationFactory) {
		if (mForceFieldCalculationFactory) {
			PU_DELETE mForceFieldCalculationFactory;
		}
		mForceFieldCalculationFactory = forceFieldCalculationFactory;
	}
	
	ForceFieldCalculationFactory * ForceField::createForceFieldCalculationFactory(ForceFieldType type) {
		mForceFieldType = type;
		if (type == FF_MATRIX_CALC) {
			// Use precreated matrix
			setForceFieldCalculationFactory(PU_NEW MatrixForceFieldCalculationFactory());
			return getForceFieldCalculationFactory();
		} else {
			// Use realtime calculation
			setForceFieldCalculationFactory(PU_NEW RealTimeForceFieldCalculationFactory());
			return getForceFieldCalculationFactory();
		}
	}
	
	ForceField::ForceFieldType ForceField::getForceFieldType() const {
		return mForceFieldType;
	}
	
	void ForceField::setForceFieldType(const ForceField::ForceFieldType forceFieldType) {
		mForceFieldType = forceFieldType;
		if (mForceFieldCalculationFactory) {
			initialise(mForceFieldType, mForceFieldSize, mOctaves, mFrequency, mAmplitude, mPersistence, mWorldSize);
		}
	}
	
	ushort ForceField::getOctaves() const {
		return mOctaves;
	}
	
	void ForceField::setOctaves(ushort octaves) {
		mOctaves = octaves;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setOctaves(octaves);
		}
	}
	
	double ForceField::getFrequency() const {
		return mFrequency;
	}
	
	void ForceField::setFrequency(double frequency) {
		mFrequency = frequency;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setFrequency(frequency);
		}
	}
	
	double ForceField::getAmplitude() const {
		return mAmplitude;
	}
	
	void ForceField::setAmplitude(double amplitude) {
		mAmplitude = amplitude;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setAmplitude(amplitude);
		}
	}
	
	double ForceField::getPersistence() const {
		return mPersistence;
	}
	
	void ForceField::setPersistence(double persistence) {
		mPersistence = persistence;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setPersistence(persistence);
		}
	}
	
	unsigned int ForceField::getForceFieldSize() const {
		return mForceFieldSize;
	}
	
	void ForceField::setForceFieldSize(unsigned int forceFieldSize) {
		mForceFieldSize = forceFieldSize;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setForceFieldSize(forceFieldSize);
		}
	}
	
	Vector3 ForceField::getWorldSize() const {
		return mWorldSize;
	}
	
	void ForceField::setWorldSize(const Vector3 & worldSize) {
		mWorldSize = worldSize;
		if (mForceFieldCalculationFactory) {
			mForceFieldCalculationFactory->setWorldSize(worldSize);
		}
	}

} /* namespace ParticleUniverse */
