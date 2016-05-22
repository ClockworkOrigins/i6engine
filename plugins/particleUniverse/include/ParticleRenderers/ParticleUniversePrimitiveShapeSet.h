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

#ifndef __PU_PRIMITIVE_SHAPE_SET_H__
#define __PU_PRIMITIVE_SHAPE_SET_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAxisAlignedBox.h"
#include "ParticleUniverseCamera.h"

#include "OgreMovableObject.h"
#include "OgreRenderable.h"

namespace ParticleUniverse {

	/** The PrimitiveShapeSet is comparable with the BillboardSet in Ogre and provides a virtual class in which some
		basic functions are implemented. This class cannot be used on its own, but must always be subclassed.
    */
	class _ParticleUniverseExport PrimitiveShapeSet : public Ogre::MovableObject, public Ogre::Renderable {
	public:
		/** Default constructor.
		*/
		PrimitiveShapeSet(const String & name, unsigned int poolSize = 20, bool externalData = false);

		/** Default constructor.
		*/
		PrimitiveShapeSet();

		/** Default destructor.
		*/
	    virtual ~PrimitiveShapeSet();

		/** Set indication whether the 'Z' rotation is activated. Z rotation is a 2D rotation effect and
			for a primitive shapeset implemented as a texture rotation.
		*/
		virtual void setZRotated(bool zRotated);

		/** Returns true if the Z rotation has been set. This causes the textures to rotate.
		*/
		virtual bool isZRotated() const;

		/** Internal callback used by primitive shapes to notify their parent that they have been rotated 
			around the Z-axis.
		*/
		virtual void _notifyZRotated();

		/** Sets the name of the material to be used for this primitive shapeset.
		*/
		virtual void setMaterialName(const String & name);

		/** Gets the name of the material to be used for this primitive shapeset.
		*/
		virtual const String & getMaterialName() const;

		/** Internal callback used by primitive shapes to notify their parent that they have been resized.
		*/
		virtual void _notifyResized();

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void _notifyCurrentCamera(Camera * cam);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual const AxisAlignedBox & getBoundingBox() const;

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual Real getBoundingRadius() const;

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual const Ogre::MaterialPtr & getMaterial() const;

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void getWorldTransforms(Ogre::Matrix4 * xform) const;

		/** @copydoc Renderable::getWorldOrientation */
		virtual const Quaternion & getWorldOrientation() const;

		/** @copydoc Renderable::getWorldPosition */
		virtual const Vector3 & getWorldPosition() const;

		/** Returns whether or not primitive shapes in this primitive shapeset are tested individually for culling.
		*/
		virtual bool isCullIndividually() const;

		/** Sets whether culling tests primitive shapes in this primitive shapeset individually as well as in a group.
		*/
		virtual void setCullIndividually(bool cullIndividual);

		/** Overridden, see Renderable
		*/
		virtual Real getSquaredViewDepth(const Camera * cam) const;

		/** @copydoc Renderable::getLights 
		*/
		virtual const Ogre::LightList & getLights() const;

		/** Override to return specific type flag
		*/
		virtual uint32 getTypeFlags() const;

		/** Rotate Texture
		*/
		virtual void rotateTexture(Real speed);

		/** @see MovableObject
		*/
		virtual void visitRenderables(Ogre::Renderable::Visitor * visitor, bool debugRenderables = false) { /* No implementation */ }

	protected:
		/** Bounds of all billboards in this set
		*/
		AxisAlignedBox mAABB;

		/** Bounding radius
		*/
		Real mBoundingRadius;

		/** Name of the material to use
		*/
		String mMaterialName;

		/** Pointer to the material to use
		*/
		Ogre::MaterialPtr mpMaterial;

		/** Flag indicating whether each billboard should be culled separately (default: false)
		*/
		bool mCullIndividual;

		/**
		*/
		bool mWorldSpace;

		/** Current camera
		*/
		Camera * mCurrentCamera;

		/** Indication whether the Z rotation is active or not
		*/
		bool mZRotated;

		/** True if no primitive shape in this set has been resized - greater efficiency.
		*/
		bool mAllDefaultSize;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PRIMITIVE_SHAPE_SET_H__ */
