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

#ifndef __PU_BOX_SET_H__
#define __PU_BOX_SET_H__

#include "ParticleRenderers/ParticleUniversePrimitiveShapeSet.h"

namespace ParticleUniverse {

	class Box;

	/** The BoxSet and Box classes are comparable with the BillboardSet and Billboard classes in Ogre, instead
		Boxes are rendered.
	@remarks
		The uv mapping is done like this:
<pre>
|---|---|
| 1 | 4 |
|---|---|
| 2 | 5 |
|---|---|
| 3 | 6 |
|---|---|
</pre>
    */
	class _ParticleUniverseExport BoxSet : public PrimitiveShapeSet {
	public:
		/** Default constructor.
		*/
		BoxSet(const String & name, unsigned int poolSize = 20, bool externalData = false);

		/** Default constructor.
		*/
		BoxSet();

		/** Default destructor.
		*/
	    virtual ~BoxSet();

		/** Creates a new box and adds it to this set.
		*/
		Box * createBox(const Vector3 & position);

		/** Creates a new box and adds it to this set.
		*/
		Box * createBox(Real x, Real y, Real z);

		/** Returns the number of active boxes which currently make up this set.
		*/
		unsigned int getNumBoxes() const;
			
		/** Tells the set whether to allow automatic extension of the pool of boxes.
		*/
		void setAutoextend(bool autoextend);
			
		/** Returns true if the box pool automatically extends.
		*/
		bool isAutoextend() const;

		/** Adjusts the size of the pool of boxes available in this set.
		*/
		void setPoolSize(size_t size);

		/** Returns the current size of the box pool.
		*/
		unsigned int getPoolSize() const;

		/** Empties this set of all boxes.
		*/
		void clear();

		/** Returns a pointer to the box at the supplied index.
		*/
		Box * getBox(unsigned int index) const;

		/** Removes the box at the supplied index.
		*/
		void removeBox(unsigned int index);

		/** Removes a box from the set.
		*/
		void removeBox(Box * box);

		/** Sets the default dimensions of the boxes in this set.
		*/
		void setDefaultDimensions(Real width, Real height, Real depth);

		/** See setDefaultDimensions - this sets 1 component individually.
		*/
		void setDefaultWidth(Real width);

		/** See setDefaultDimensions - this gets 1 component individually.
		*/
		Real getDefaultWidth() const;

		/** See setDefaultDimensions - this sets 1 component individually.
		*/
        void setDefaultHeight(Real height);

		/** See setDefaultDimensions - this gets 1 component individually.
		*/
		Real getDefaultHeight() const;

		/**  See setDefaultDimensions - this gets 1 component individually.
		*/
        void setDefaultDepth(Real depth);

		/** See setDefaultDimensions - this gets 1 component individually.
		*/
		Real getDefaultDepth() const;

		/** Begin injection of box data; applicable when constructing the BoxSet for external data use.
		*/
		void beginBoxes(size_t numBoxes = 0);

		/** Define a box.
		*/
		void injectBox(Box & bb);
        
		/** Finish defining boxes. */
		void endBoxes();

		/** Set the bounds of the BoxSet.
		*/
		void setBounds(const AxisAlignedBox & box, Real radius);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void _updateRenderQueue(Ogre::RenderQueue * queue);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void getRenderOperation(Ogre::RenderOperation & op);

		/** Overridden from MovableObject 
		*/
		virtual const String & getMovableType() const;

		/** Update the bounds of the box set
		*/
		virtual void _updateBounds();

		/** Sets whether boxes should be treated as being in world space. 
		*/
		void setBoxesInWorldSpace(bool ws) { mWorldSpace = ws; }

	protected:
		/** Default width of each box
		*/
		Real mDefaultWidth;

		/** Default height of each box
		*/
		Real mDefaultHeight;

		/** Default depth of each box
		*/
		Real mDefaultDepth;

		/** Half values
		*/
		Real mDefaultHalfWidth;
		Real mDefaultHalfHeight;
		Real mDefaultHalfDepth;

		/** Texture coordinates.
		*/
		Vector2 mTextCoord[16];

		/** Flag indicating whether to autoextend pool
		*/
		bool mAutoExtendPool;

		/** Contains the local positions of the box corners, repeated because of texture mapping.
		*/
		Vector3 mDefaultCorners[16];

		typedef list<Box *> ActiveBoxList;
		typedef list<Box *> FreeBoxList;
		typedef vector<Box *> BoxPool;

		/** Active box list.
		@remarks
		This is a linked list of pointers to boxes in the box pool.
		*/
		ActiveBoxList mActiveBoxes;

		/** Free box queue.
		*/
		FreeBoxList mFreeBoxes;

		/** Pool of box instances for use and reuse in the active box list.
		*/
		BoxPool mBoxPool;

		/** The vertex position data for all boxes in this set.
		*/
		Ogre::VertexData * mVertexData;

		/** Shortcut to main buffer
		*/
		Ogre::HardwareVertexBufferSharedPtr mMainBuf;

		/** Locked pointer to buffer
		*/
		float * mLockPtr;

		/** The vertex index data for all boxes in this set (1 set only)
		*/
		Ogre::IndexData * mIndexData;

		typedef vector<Ogre::FloatRect> TextureCoordSets;
		TextureCoordSets mTextureCoords;

		/** Internal method for culling individual boxes
		*/
		inline bool boxVisible(Camera * cam, const Box & box);

		/** Number of visible boxes
		*/
		unsigned short mNumVisibleBoxes;

		/** Internal method for increasing pool size
		*/
		virtual void increasePool(size_t size);

		/**
		*/
		void _genDefaultCorners();

		/** Internal method for generating vertex data
		*/
		void _genVertices(Box & box);

		/** Internal method for generating texture coordinates
		*/
		void _initTextCoord();

	private:
		/** Flag indicating whether the HW buffers have been created.
		*/
		bool mBuffersCreated;

		/** The number of boxes in the pool.
		*/
		size_t mPoolSize;

		/** Is external box data in use?
		*/
		bool mExternalData;

		/** Internal method creates vertex and index buffers.
		*/
		void _createBuffers();

		/** Internal method destroys vertex and index buffers.
		*/
		void _destroyBuffers();
	};

	/** Factory object for creating BoxSet instances */
	class _ParticleUniverseExport BoxSetFactory : public Ogre::MovableObjectFactory {
	public:
		static String PU_FACTORY_TYPE_NAME;

		BoxSetFactory() {}
		~BoxSetFactory() {}

		const String & getType() const;
		void destroyInstance( Ogre::MovableObject * obj);

	protected:
		Ogre::MovableObject * createInstanceImpl(const String & name, const Ogre::NameValuePairList * params);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_SET_H__ */
