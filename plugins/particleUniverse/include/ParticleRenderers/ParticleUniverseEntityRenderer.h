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
