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

#include "ParticleRenderers/ParticleUniverseSphereSet.h"

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleUniverseSimpleSphere.h"

#include "ParticleRenderers/ParticleUniverseSphere.h"

#include "OgreCamera.h"
#include "OgreHardwareBufferManager.h"
#include "OgreNode.h"
#include "OgreRoot.h"

namespace ParticleUniverse {
	
	SphereSet::SphereSet() : PrimitiveShapeSet(), mDefaultRadius(20), mAutoExtendPool(true), mVertexData(nullptr), mLockPtr(nullptr), pIdx(nullptr), mIndexData(nullptr), mNumberOfRings(16), mNumberOfSegments(16), mVertexIndex(1), mBuffersCreated(false), mPoolSize(0), mExternalData(false) {
		setNumberOfRings(16);
		setNumberOfSegments(16);
	}
	
	SphereSet::SphereSet(const String & name, unsigned int poolSize, bool externalData) : PrimitiveShapeSet(name, poolSize, externalData), mDefaultRadius(20), mAutoExtendPool(true), mVertexData(nullptr), mLockPtr(nullptr), pIdx(nullptr), mIndexData(nullptr), mNumberOfRings(16), mNumberOfSegments(16), mVertexIndex(1), mBuffersCreated(false), mPoolSize(poolSize), mExternalData(externalData) {
		setPoolSize(poolSize);
		setNumberOfRings(16);
		setNumberOfSegments(16);
	}
	
	SphereSet::~SphereSet() {
		// Free pool items
		for (SpherePool::iterator i = mSpherePool.begin(); i != mSpherePool.end(); ++i) {
			PU_DELETE *i;
		}

		// Delete buffers
		_destroyBuffers();
	}
    
	Sphere * SphereSet::createSphere(const Vector3 & position) {
		if (mFreeSpheres.empty()) {
			if (mAutoExtendPool) {
				setPoolSize(getPoolSize() * 2);
            } else {
				return nullptr;
			}
		}

        // Get a new sphere
        Sphere * newSphere = mFreeSpheres.front();
		mActiveSpheres.splice(mActiveSpheres.end(), mFreeSpheres, mFreeSpheres.begin());
		newSphere->setPosition(position);
        newSphere->resetRadius();
        newSphere->_notifyOwner(this);

		// Merge
        Vector3 vecAdjust(mDefaultRadius, mDefaultRadius, mDefaultRadius);
		Vector3 newMin = position - vecAdjust;
		Vector3 newMax = position + vecAdjust;
        mAABB.merge(newMin);
        mAABB.merge(newMax);
		mBoundingRadius = std::max(mBoundingRadius, mDefaultRadius);

		return newSphere;
	}
	
	Sphere * SphereSet::createSphere(Real x, Real y, Real z) {
		return createSphere(Vector3( x, y, z ));
	}
	
	unsigned int SphereSet::getNumSpheres() const {
		return static_cast<unsigned int>(mActiveSpheres.size());
	}
	
	void SphereSet::clear() {
		mFreeSpheres.splice(mFreeSpheres.end(), mActiveSpheres);
	}
	
	Sphere * SphereSet::getSphere(unsigned int index) const {
		assert(index < mActiveSpheres.size() && "Sphere index out of bounds." );

        ActiveSphereList::const_iterator it;
		if (index >= (mActiveSpheres.size() >> 1)) {
			index = static_cast<unsigned int>(mActiveSpheres.size()) - index;
			for (it = mActiveSpheres.end(); index; --index, --it);
		} else {
			for (it = mActiveSpheres.begin(); index; --index, ++it);
		}

		return *it;
    }
	
	void SphereSet::removeSphere(unsigned int index) {
		assert(index < mActiveSpheres.size() && "Sphere index out of bounds." );

        ActiveSphereList::iterator it;
        if (index >= (mActiveSpheres.size() >> 1 )) {
			index = static_cast<unsigned int>(mActiveSpheres.size()) - index;
			for (it = mActiveSpheres.end(); index; --index, --it);
		} else {
			for (it = mActiveSpheres.begin(); index; --index, ++it);
		}

		mFreeSpheres.splice(mFreeSpheres.end(), mActiveSpheres, it);
    }
	
	void SphereSet::removeSphere(Sphere * sphere) {
		ActiveSphereList::iterator it = std::find(mActiveSpheres.begin(), mActiveSpheres.end(), sphere);
		assert(it != mActiveSpheres.end() && "Sphere isn't in the active list." );

		mFreeSpheres.splice(mFreeSpheres.end(), mActiveSpheres, it);
	}
	
	void SphereSet::setDefaultRadius(Real radius) {
		mDefaultRadius = radius;
	}
	
	Real SphereSet::getDefaultRadius() const {
		return mDefaultRadius;
	}
	
	void SphereSet::setNumberOfRings(unsigned int numberOfRings) {
		mNumberOfRings = numberOfRings;
		_calculateVertexAndIndexPerSphere();
	}
	
	unsigned int SphereSet::getNumberOfRings() const {
		return mNumberOfRings;
	}
	
	void SphereSet::setNumberOfSegments(unsigned int numberOfSegments) {
		mNumberOfSegments = numberOfSegments;
		_calculateVertexAndIndexPerSphere();
	}
	
	unsigned int SphereSet::getNumberOfSegments() const {
		return mNumberOfSegments;
	}
	
	void SphereSet::_calculateVertexAndIndexPerSphere() {
		mVertexCountPerSphere = (mNumberOfRings + 1) * (mNumberOfSegments + 1);
		mIndexCountPerSphere = 6 * mNumberOfRings * (mNumberOfSegments + 1);
	}
	
	void SphereSet::beginSpheres(size_t numSpheres) {
		// Create vertex / index buffers
		if (!mBuffersCreated) {
			_createBuffers();
		}

        mNumVisibleSpheres = 0;
		mVertexIndex = 1;

		if (numSpheres) {
			numSpheres = std::min(mPoolSize, numSpheres);
			size_t sphereVertexSize = mMainBuf->getVertexSize() * mVertexCountPerSphere;
			assert (numSpheres * sphereVertexSize <= mMainBuf->getSizeInBytes());

			mLockPtr = static_cast<float *>(mMainBuf->lock(0, numSpheres * sphereVertexSize, Ogre::HardwareBuffer::HBL_DISCARD));

			size_t sphereIndexSize = mIndexData->indexBuffer->getIndexSize() * mIndexCountPerSphere;
			assert (numSpheres * sphereIndexSize <= mIndexData->indexBuffer->getSizeInBytes());

			pIdx = static_cast<ushort *>(mIndexData->indexBuffer->lock(0, numSpheres * sphereIndexSize, Ogre::HardwareBuffer::HBL_DISCARD));
		} else {
			mLockPtr = static_cast<float *>(mMainBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

			pIdx = static_cast<ushort *>(mIndexData->indexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		}
	}
	
	void SphereSet::injectSphere(Sphere & sphere) {
		if (mNumVisibleSpheres == mPoolSize) {
			return;
		}

		if (!sphereVisible(mCurrentCamera, sphere)) {
			return;
		}

		// Generate vertices
		_genVertices(sphere);
        
		mNumVisibleSpheres++;
	}
    
	void SphereSet::endSpheres() {
		mMainBuf->unlock();
		mIndexData->indexBuffer->unlock();
	}
	
	void SphereSet::setBounds(const AxisAlignedBox & box, Real radius) {
		mAABB = box;
		mBoundingRadius = radius;
	}
    
    void SphereSet::_updateBounds()  {
        if (mActiveSpheres.empty()) {
            // No spheres
            mAABB.setNull();
			mBoundingRadius = 0.0;
        } else {
			Real maxSqLen = -1.0;

            Vector3 min(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);
            Vector3 max(Math::NEG_INFINITY, Math::NEG_INFINITY, Math::NEG_INFINITY);

			for (ActiveSphereList::iterator i = mActiveSpheres.begin(); i != mActiveSpheres.end(); ++i) {
                const Vector3 & pos = (*i)->getPosition();
                min.makeFloor(pos);
                max.makeCeil(pos);

				maxSqLen = std::max(maxSqLen, pos.squaredLength());
            }

            Vector3 vecAdjust(mDefaultRadius, mDefaultRadius, mDefaultRadius);
            min -= vecAdjust;
            max += vecAdjust;

            mAABB.setExtents(min, max);
			mBoundingRadius = Math::Sqrt(maxSqLen);
        }

		if (mParentNode) {
			mParentNode->needUpdate();
		}

    }
	
	void SphereSet::_updateRenderQueue(Ogre::RenderQueue * queue)  {
		if (!mExternalData) {
			beginSpheres(mActiveSpheres.size());
			for (ActiveSphereList::iterator it = mActiveSpheres.begin(); it != mActiveSpheres.end(); ++it) {
				injectSphere(*(*it));
			}
			endSpheres();
		}

		if (mRenderQueueIDSet) {
			queue->addRenderable(this, mRenderQueueID);
		} else {
			queue->addRenderable(this);
		}
	}
	
	void SphereSet::getRenderOperation(Ogre::RenderOperation & op) {
		op.vertexData = mVertexData;
		op.vertexData->vertexStart = 0;
		op.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		op.useIndexes = true;
		op.vertexData->vertexCount = mNumVisibleSpheres * mVertexCountPerSphere;
		op.indexData = mIndexData;
		op.indexData->indexCount = mNumVisibleSpheres * mIndexCountPerSphere;
		op.indexData->indexStart = 0;
	}
	
	void SphereSet::setAutoextend(bool autoextend) {
		mAutoExtendPool = autoextend;
	}
	
	bool SphereSet::isAutoextend() const {
		return mAutoExtendPool;
	}
	
	void SphereSet::setPoolSize(size_t size) {
		if (!mExternalData) {
			size_t currSize = mSpherePool.size();
			if (currSize >= size) {
				return;
			}

			this->increasePool(size);

			for (size_t i = currSize; i < size; ++i) {
				// Add new items to the queue
				mFreeSpheres.push_back(mSpherePool[i]);
			}
		}

		mPoolSize = size;
		_destroyBuffers();
	}
	
	void SphereSet::_createBuffers() {
		// Create vertex buffer
		mVertexData = PU_NEW Ogre::VertexData();
		mVertexData->vertexCount = mPoolSize * mVertexCountPerSphere;

        mVertexData->vertexStart = 0;
		Ogre::VertexDeclaration * decl = mVertexData->vertexDeclaration;
		Ogre::VertexBufferBinding * binding = mVertexData->vertexBufferBinding;

		// Create Vertices, Normals, Colour and Texture Coordinates
		size_t offset = 0;
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);
		decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);

		mMainBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(0), mVertexData->vertexCount, Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

		// Bind
		binding->setBinding(0, mMainBuf);

		// Create indices
		const size_t ibufCount = mIndexCountPerSphere;
		mIndexData  = PU_NEW Ogre::IndexData();
		mIndexData->indexStart = 0;
		mIndexData->indexCount = mPoolSize * ibufCount;
		mIndexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, mIndexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

        mBuffersCreated = true;
	}
	
	void SphereSet::_destroyBuffers() {
		if (mVertexData) {
			PU_DELETE mVertexData;
			mVertexData = nullptr;
		}
		if (mIndexData) {
			PU_DELETE mIndexData;
			mIndexData = nullptr;
		}

		mMainBuf.setNull();

		mBuffersCreated = false;
	}
	
	unsigned int SphereSet::getPoolSize() const {
		return static_cast<unsigned int>(mSpherePool.size());
	}
	
	bool SphereSet::sphereVisible(Camera * cam, const Sphere & sphere) {
		// If not individually culled it is always visible
		if (!mCullIndividual) {
			return true;
		}

		// Culling based on sphere
		SimpleSphere sph;
		Ogre::Matrix4 xworld;
		getWorldTransforms(&xworld);
		sph.setCenter(xworld.transformAffine(sphere.mPosition));

		if (sphere.mOwnRadius) {
			sph.setRadius(sphere.getOwnRadius());
		} else {
			sph.setRadius(getDefaultRadius());
		}

		return cam->isVisible(sph);
	}
	
	void SphereSet::increasePool(size_t size) {
		size_t oldSize = mSpherePool.size();

		// Increase size
		mSpherePool.reserve(size);
		mSpherePool.resize(size);

		// Create new spheres
		for (size_t i = oldSize; i < size; ++i) {
			mSpherePool[i] = PU_NEW Sphere();
		}
	}
	
	void SphereSet::_genVertices(Sphere & sphere) {
		Ogre::RGBA colour;
		Ogre::Root::getSingleton().convertColourValue(sphere.mColour, &colour);
		Ogre::RGBA * pCol;
		Real radius = getDefaultRadius();
		if (sphere.mOwnRadius) {
			radius = sphere.getOwnRadius();
		}

		Real stepRingAngle = (Math::PI / mNumberOfRings);
		Real stepSegmentAngle = (2 * Math::PI / mNumberOfSegments);

		for (unsigned int ring = 0; ring <= mNumberOfRings; ring++) {
			Real r0 = radius * sinf (ring * stepRingAngle);
			Real y0 = radius * cosf (ring * stepRingAngle);

			for (unsigned int segment = 0; segment <= mNumberOfSegments; segment++) {
				Real x0 = r0 * sinf(segment * stepSegmentAngle);
				Real z0 = r0 * cosf(segment * stepSegmentAngle);

				// Vertex
				Vector3 orientedVertex = sphere.mOrientation * Vector3(x0, y0, z0);
				*mLockPtr++ = float(orientedVertex.x + sphere.mPosition.x);
				*mLockPtr++ = float(orientedVertex.y + sphere.mPosition.y);
				*mLockPtr++ = float(orientedVertex.z + sphere.mPosition.z);

				// Normal
				Vector3 vNormal = orientedVertex.normalisedCopy();
				*mLockPtr++ = float(vNormal.x);
				*mLockPtr++ = float(vNormal.y);
				*mLockPtr++ = float(vNormal.z);

				// Colour
				pCol = static_cast<Ogre::RGBA *>(static_cast<void *>(mLockPtr));
				*pCol++ = colour;
				mLockPtr = static_cast<float *>(static_cast<void *>(pCol));

				// Texture Coordinates
				*mLockPtr++ = float(segment) / float(mNumberOfSegments);
				*mLockPtr++ = float(ring) / float(mNumberOfRings);

				if (ring != mNumberOfRings) {
					// each vertex (except the last) has six indices pointing to it
					*pIdx++ = ushort(mVertexIndex + mNumberOfSegments + 1);
					*pIdx++ = mVertexIndex;
					*pIdx++ = ushort(mVertexIndex + mNumberOfSegments);
					*pIdx++ = ushort(mVertexIndex + mNumberOfSegments + 1);
					*pIdx++ = mVertexIndex + 1;
					*pIdx++ = mVertexIndex;
				}
				mVertexIndex++;
			}
		}
	}
	
	const String & SphereSet::getMovableType() const {
		return SphereSetFactory::PU_FACTORY_TYPE_NAME;
	}
	
	String SphereSetFactory::PU_FACTORY_TYPE_NAME = "SphereSet";
	
	const String & SphereSetFactory::getType() const {
		return PU_FACTORY_TYPE_NAME;
	}
	
	Ogre::MovableObject * SphereSetFactory::createInstanceImpl(const String & name, const Ogre::NameValuePairList * params) {
		bool externalData = false;
		unsigned int poolSize = 0;

		if (params != nullptr) {
			Ogre::NameValuePairList::const_iterator ni = params->find("poolSize");
			if (ni != params->end()) {
				poolSize = StringConverter::parseUnsignedInt(ni->second);
			}
			ni = params->find("externalData");
			if (ni != params->end()) {
				externalData = StringConverter::parseBool(ni->second);
			}
		}

		if (poolSize > 0) {
			return PU_NEW SphereSet(name, poolSize, externalData);
		} else {
			return PU_NEW SphereSet(name);
		}
	}
	
	void SphereSetFactory::destroyInstance(Ogre::MovableObject * obj) {
		PU_DELETE obj;
	}

} /* namespace ParticleUniverse */
