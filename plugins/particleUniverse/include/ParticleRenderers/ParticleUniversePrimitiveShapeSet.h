/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
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
