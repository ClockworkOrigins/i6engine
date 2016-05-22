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

#include "ParticleEmitters/ParticleUniverseVertexEmitter.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseTechnique.h"

#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreSubMesh.h"

namespace ParticleUniverse {

	// Constants
	const unsigned short VertexEmitter::DEFAULT_STEP = 1;
	const unsigned short VertexEmitter::DEFAULT_SEGMENTS = 1;
	const unsigned short VertexEmitter::DEFAULT_ITERATIONS = 1;

	VertexEmitter::VertexEmitter() : ParticleEmitter(), mPositionElement(nullptr), mVertexData(nullptr), mVertexBuffer(nullptr), mMesh(nullptr), mSubMesh(nullptr), mMeshProcessed(false), mVertexSize(12), mMaxVertexCount(0), mVertexCount(0), mSubMeshNumber(0), mAllVerticesProcessed(false), mStep(DEFAULT_STEP), mSegments(DEFAULT_SEGMENTS), mIterations(DEFAULT_ITERATIONS), mMeshName(BLANK_STRING) {
		mBuffer.setNull();
	}
	
	unsigned short VertexEmitter::getIterations() const {
		return mIterations;
	}
	
	void VertexEmitter::setIterations(unsigned short iterations) {
		mIterations = iterations;
	}
	
	unsigned short VertexEmitter::getSegments() const {
		return mSegments;
	}
	
	void VertexEmitter::setSegments(unsigned short segments) {
		mSegments = segments;
	}
	
	unsigned short VertexEmitter::getStep() const {
		return mStep;
	}
	
	void VertexEmitter::setStep(unsigned short step) {
		mStep = step;
	}
	
	const String & VertexEmitter::getMeshName() const {
		return mMeshName;
	}
	
	void VertexEmitter::setMeshName(const String & meshName) {
		_setDefaults(); // Triggers loading the new mesh
		mMeshName = meshName;
	}
	
	void VertexEmitter::_notifyStart() {
		ParticleEmitter::_notifyStart();
		_setDefaults();
	}
	
	unsigned short VertexEmitter::_calculateRequestedParticles(Real timeElapsed) {
		// Only return the requested number if the emitter can actually emit them
		unsigned short requested = ParticleEmitter::_calculateRequestedParticles(timeElapsed);
		if (requested <= mSpawnPositionList.size()) {
			return requested;
		}

		return 0;
	}
	
	void VertexEmitter::_initParticlePosition(Particle * particle) {
		// Get the first one from the list.
		// The _calculateRequestedParticles() ensures that there are particles in the list.
		// All positions in mSpawnPositionList are in localspace
		ParticleSystem * sys = mParentTechnique->getParentSystem();
		if (sys) {
			particle->position = getDerivedPosition() + sys->getDerivedOrientation() * (_mEmitterScale * *mSpawnPositionList.begin());
		} else {
			particle->position = getDerivedPosition() + _mEmitterScale * *mSpawnPositionList.begin();
		}
			
		mSpawnPositionList.pop_front();
		particle->originalPosition = particle->position;
	}
	
	void VertexEmitter::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		if (mAllVerticesProcessed) {
			return;
		}

		// Generate the next few spawn points if not all vertices are processed.
		for (unsigned short i = 0; i < mIterations; ++i) {
			_generatePoints();
			if (mAllVerticesProcessed) {
				return;
			}
		}

		// Calculate the emitters position
		getDerivedPosition();
	}
	
	void VertexEmitter::_setDefaults() {
		// Set the default values except the name of the mesh, which may only be changed by the 
		// setMeshName() function
		mSpawnPositionList.clear();
		mPositionElement = nullptr;
		mVertexData = nullptr;
		mVertexBuffer = nullptr;
		mMesh = nullptr;
		mSubMesh = nullptr;
		mMaxVertexCount = 0;
		mVertexCount = 0;
		mMeshProcessed = false;
		mAllVerticesProcessed = false;
		mSubMeshNumber = 0;

		if (!mBuffer.isNull()) {
			if (mBuffer->isLocked()) {
				mBuffer->unlock();
			}
			mBuffer.setNull();
		}
	}
	
	void VertexEmitter::_loadMesh() {
		Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load(mMeshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mMesh = mesh.getPointer();
		mMeshProcessed = false;
			
		if (mMesh->getNumSubMeshes() > 0) {
			mSubMesh = mMesh->getSubMesh(0);
		} else {
			mSubMesh = nullptr;
		}
	}
	
	void VertexEmitter::_generatePoints() {
		if (mAllVerticesProcessed) {
			return;
		}

		if (!mMesh && mMeshName != BLANK_STRING) {
			_loadMesh();
		}

		// At least process the vertices of the mesh itself and then all its submeshes
		if (!mMeshProcessed) {
			_generatePointsFromMesh();
		} else {
			if (mSubMesh && !mSubMesh->useSharedVertices) {
				_generatePointsFromSubMesh();
			}
		}

		if (mVertexCount > 0 && mVertexCount >= mMaxVertexCount) {
			// The vertices of 1 mesh/subMesh are processed. Determine whether more submeshes need to
			// be processed.
			mBuffer->unlock();
			mBuffer.setNull();
			mVertexCount = 0;
			mMaxVertexCount = 0;
			mVertexData = nullptr;
			mVertexBuffer = nullptr;
			mSubMeshNumber++;
			if (mSubMeshNumber >= mMesh->getNumSubMeshes()) {
				mAllVerticesProcessed = true;
			} else {
				mSubMesh = mMesh->getSubMesh(mSubMeshNumber);
			}
		}
	}
	
	void VertexEmitter::_generatePointsFromMesh() {
		if (mMaxVertexCount == 0 && mMesh) {
			mVertexData = mMesh->sharedVertexData;
			if (!mVertexData) {
				mMeshProcessed = true;
				return;
			}
			_fillVertexBuffer();
		}

		_generatePointsFromVertexBuffer();
		if (mVertexCount >= mMaxVertexCount) {
			mMeshProcessed = true;
		}
	}
	
	void VertexEmitter::_generatePointsFromSubMesh() {
		if (mMaxVertexCount == 0 && mSubMesh) {
			mVertexData = mSubMesh->vertexData;
			if (!mVertexData) {
				return;
			}
			_fillVertexBuffer();
		}

		_generatePointsFromVertexBuffer();
	}	
	
	void VertexEmitter::_fillVertexBuffer() {
		mVertexCount = 0;
		mMaxVertexCount = mVertexData->vertexCount - mStep;
		mPositionElement = mVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		mBuffer = mVertexData->vertexBufferBinding->getBuffer(mPositionElement->getSource());
		mVertexSize = mBuffer->getVertexSize();
		mVertexBuffer = static_cast<unsigned char *>(mBuffer->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
	}
	
	void VertexEmitter::_generatePointsFromVertexBuffer() {
		if (mMaxVertexCount > 1) {
			float * pReal;
			mPositionElement->baseVertexPointerToElement(mVertexBuffer, &pReal);
			Vector3 v1(pReal[0], pReal[1], pReal[2]);
			mVertexBuffer += mStep * mVertexSize; // No validation; assume that it doesn't exceed.
			mPositionElement->baseVertexPointerToElement(mVertexBuffer, &pReal);
			Vector3 v2(pReal[0], pReal[1], pReal[2]);
			_generatePoints (v1, v2);
			mVertexCount += mStep;
		}
	}
	
	void VertexEmitter::_generatePoints(const Vector3 & startVector, const Vector3 & endVector, Real lengthIncrement) {
		/*  Two possible ways of generating spawn points.
			1. Generate (mSegments + 2) Vectors (including start and end vector), equally distributed.
			2. Generate number of Vectors depending on the length between startVector and endVector. The
			   longer the length, the more vectors are generated.

			The distribution of the spawn points in the list is as followed.
			first iteration p1, 3 points:
			- p1.3, p1.2, p1.1
			second iteration, p2, 4 points:
			- p2.4, p1.3, p2.3, p1.2, p2.2, p1.1, p2.1
			second iteration, p3, 3 points:
			- p3.3, p2.4, p3.2, p1.3, p3.1, p2.3, p1.2, p2.2, p1.1, p2.1
		@remarks
			Use objectspace for the vectors and only transform as soon as a particle is emitted.
		*/
		list<Vector3>::iterator iterator = mSpawnPositionList.end();
		Vector3 start = startVector;
		if (lengthIncrement > 0.0) {
			// Determine number of dividers, based on the length increment
			mSegments = static_cast<unsigned short>(((endVector - startVector).length()) / lengthIncrement - 1);
		}
        Vector3 increment = (endVector - startVector) / mSegments;
		
		for (unsigned short i = 0; i <= mSegments; i++) {
			mSpawnPositionList.insert (iterator, start);
			start += increment;
			iterator--;
			if (iterator != mSpawnPositionList.begin()) {
				iterator--;
			}
		}
	}
	
	void VertexEmitter::copyAttributesTo(ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);

		VertexEmitter * vertexEmitter = static_cast<VertexEmitter *>(emitter);
		vertexEmitter->mStep = mStep;
		vertexEmitter->mSegments = mSegments;
		vertexEmitter->mIterations = mIterations;
		vertexEmitter->mMeshName = mMeshName;
	}

} /* namespace ParticleUniverse */
