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

#ifndef __PU_SCENE_DECORATOR_EXTERN_H__
#define __PU_SCENE_DECORATOR_EXTERN_H__

#include "ParticleUniverseExtern.h"

namespace ParticleUniverse {

	/** The SceneDecoratorExtern is a test class that can be used to add additional objects to the scene. This allows quick
		prototyping, but it it not really usable in a real situation.
    */
	class _ParticleUniverseExport SceneDecoratorExtern : public Extern {
	public:
		SceneDecoratorExtern();
	    virtual ~SceneDecoratorExtern() {}

		/** see Extern::_prepare
		*/
		virtual void _prepare(ParticleTechnique * technique);

		/** see Extern::_unprepare
		*/
		virtual void _unprepare(ParticleTechnique * technique);

		/** see Extern::_interface
		*/
		virtual void _interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed);

		/** Create the desired entity
	    */
		void createEntity();

		/** Destroy the entity again
	    */
		void destroyEntity();

		/** Return the name of the mesh
	    */
		const String & getMeshName() const;

		/** Set the name of the mesh
	    */
		void setMeshName(const String & meshName);

		/** Return the name of the material used for the entity.
	    */
		const String & getMaterialName() const;

		/** Set the name of the material used for the entity.
	    */
		void setMaterialName(const String & materialName);

		/** Returns the scale value to which the node must be set.
	    */
		const Vector3 & getScale() const;

		/** Sets the scale value to which the node must be set.
	    */
		void setScale(Vector3 & scale);

		/** Returns the position value to which the node must be set.
	    */
		const Vector3 & getPosition() const;

		/** Sets the position value to which the node must be set.
	    */
		void setPosition(Vector3 & position);

		/** See Extern::_notifyStart.
		*/
		virtual void _notifyStart();

		/** See Extern::_notifyStop.
		*/
		virtual void _notifyStop();

		/** See Extern:copyAttributesTo
	    */
		virtual void copyAttributesTo(Extern * externObject);

	protected:
		bool mEntitySet;
		Ogre::Entity * mEntity;
		String mMeshName;
		String mEntityName;
		String mMaterialName;
		bool mMaterialNameSet;
		Vector3 mScale;
		Vector3 mPosition;
		Ogre::SceneNode * mSubnode;
		unsigned int mCount;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_SCENE_DECORATOR_EXTERN_H__ */
