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

#ifndef __PU_ENTITY_RENDERER_H__
#define __PU_ENTITY_RENDERER_H__

#include "ParticleUniverseIVisualData.h"
#include "ParticleUniverseRenderer.h"

namespace ParticleUniverse {

	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport EntityRendererVisualData : public IVisualData {
	public:
		Ogre::SceneNode * node;

		EntityRendererVisualData(Ogre::SceneNode * sceneNode);

		virtual void setVisible(bool visible);
	};

	/** The EntityRenderer class is responsible to render particles as an Entity.
    */
	class _ParticleUniverseExport EntityRenderer : public ParticleRenderer {
	public:
		enum EntityOrientationType {
			ENT_ORIENTED_SELF,
			ENT_ORIENTED_SELF_MIRRORED,
			ENT_ORIENTED_SHAPE
		};

		// Constants
		static const EntityOrientationType DEFAULT_ORIENTATION_TYPE;

		EntityRenderer();
	    virtual ~EntityRenderer();

		/** Get the mesh name.
		*/
		const String & getMeshName() const;

		/** Set the mesh name.
		*/
		void setMeshName(const String & meshName);

		/** Deletes all ChildSceneNodes en Entities.
		*/
		void _destroyAll();
			
		/** Disable all ChildSceneNodes.
		*/
//			virtual void _notifyStop();
			
		/** Make all objects visible or invisible.
		*/
		virtual void setVisible(bool visible);

		/** @copydoc ParticleRenderer::_prepare */
		virtual void _prepare(ParticleTechnique * technique);

		/** @copydoc ParticleRenderer::_unprepare */
		virtual void _unprepare(ParticleTechnique * technique);

		/** 
		*/
		virtual void _updateRenderQueue(Ogre::RenderQueue * queue, ParticlePool * pool);

		/** 
		*/
		virtual void _notifyAttached(Ogre::Node * parent, bool isTagPoint = false);

		/** @copydoc ParticleRenderer::_setMaterialName */
		virtual void _setMaterialName(const String & materialName);

		/** 
		*/
		virtual void _notifyCurrentCamera(Camera * cam);
	
		/** 
		*/
		virtual void _notifyParticleQuota(size_t quota);

		/** 
		*/
		virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

		/** 
		*/
		virtual void _notifyParticleResized();

		/** 
		*/
		virtual void _notifyParticleZRotated();

		/** Rotate the textures of eacht entity.
		*/
		void _rotateTexture(VisualParticle * particle, Ogre::Entity * entity);

		/** 
		*/
		virtual void setRenderQueueGroup(uint8 queueId);

		/** 
		*/
		virtual SortMode _getSortMode() const;

		/** 
		*/
		const EntityOrientationType & getEntityOrientationType() const;

		/** 
		*/
		void setEntityOrientationType(const EntityOrientationType & entityOrientationType);

		/** 
		*/
		virtual void copyAttributesTo(ParticleRenderer * renderer);

	protected:
		vector<EntityRendererVisualData *> mAllVisualData;
		vector<EntityRendererVisualData *> mVisualData;
		vector<Ogre::Entity *> mEntities;
		size_t mQuota;
		String mMeshName;
		String mEntityName; // Used for random entity name prefix
		Real mBoxWidth;
		Real mBoxHeight;
		Real mBoxDepth;
		bool mZRotated;
		EntityOrientationType mEntityOrientationType;

		/** Make all nodes to which the entities are attached visible or invisible.
		*/
		void _makeNodesVisible(bool visible);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_ENTITY_RENDERER_H__ */
