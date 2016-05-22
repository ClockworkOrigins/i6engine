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

#ifndef __PU_ATTACHABLE_H__
#define __PU_ATTACHABLE_H__

#include "ParticleUniverseAxisAlignedBox.h"
#include "ParticleUniverseCamera.h"
#include "ParticleUniverseExtern.h"

#include "OGRE/OgreMovableObject.h"

namespace ParticleUniverse {

	/** The Attachable is a MovableObject that can be registered at a ParticleSystem as an Extern. By means
		of this construction it is possible to make a dependency between the 2 MovableObjects.
		This can be convenient if the Attachable is a Particle Affector wrapper, that affects a particle
		based on their mutual distance.
	@remarks
		This MovableObject will not be registered at the SceneManager; its purpose is to take advantage of the
		fact that it can be attached to a SceneNode. If an inherited class of Attachable uses visual (rendering)
		elements, it should use the registerAttachable() and unregisterAttachable() functions of the 
		ParticleSystemManager. This implies some additional coding, because this is not supported by the
		scripting capabilities.
    */
	class _ParticleUniverseExport Attachable : public Extern, public Ogre::MovableObject {
	public:
		// Constants
		static String PU_ATTACHABLE;

		Attachable();
	    virtual ~Attachable() {}

		/** Get the Distance Threshold
		*/
		Real getDistanceThreshold() const;

		/** Set the Distance Threshold. This threshold defines at which distance the Attachable doesn't influence 
			the particle anymore.
		*/
		void setDistanceThreshold(const Real distanceThreshold);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		void _notifyAttached(Ogre::Node * parent, bool isTagPoint = false);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void _notifyCurrentCamera(Camera * cam);

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		const String & getMovableType() const;

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		const AxisAlignedBox & getBoundingBox() const;

		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual Real getBoundingRadius() const;
			
		/** Overridden from MovableObject
		@see
			MovableObject
		*/
		virtual void _updateRenderQueue(Ogre::RenderQueue * queue);

		/** @see MovableObject
		*/
		virtual void visitRenderables(Ogre::Renderable::Visitor * visitor, bool debugRenderables = false) { /* No implementation */ }

		/** Copy attributes to another Extern object.
	    */
		virtual void copyAttributesTo(Extern * externObject);

		/** Perform initialisation actions.
	    */
		virtual void _prepare(ParticleTechnique * technique);

		/** Reverse the actions from the _prepare.
	    */
		virtual void _unprepare(ParticleTechnique * particleTechnique);

		/** Actually processes a particle.
	    */
		virtual void _interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed);

	protected:
		AxisAlignedBox mAABB;
		Real mBoundingRadius;
		Real mDistanceThreshold;
		bool mDistanceThresholdSet;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ATTACHABLE_H__ */
