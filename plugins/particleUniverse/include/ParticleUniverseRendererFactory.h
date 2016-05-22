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

#ifndef __PU_RENDERER_FACTORY_H__
#define __PU_RENDERER_FACTORY_H__

#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseRendererTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all ParticleRenderer implementations.
    */
	class _ParticleUniverseExport ParticleRendererFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
		ParticleRendererFactory() {}
		virtual ~ParticleRendererFactory() {}

		/** Returns the type of the factory, which identifies the particle Renderer type this factory creates. */
		virtual String getRendererType() const = 0;

		/** Creates a new renderer instance.
		@remarks
	    */
		virtual ParticleRenderer * createRenderer() = 0;

		/** Delete a renderer.
	    */
		void destroyRenderer (ParticleRenderer * renderer) {
			if (renderer) {
				PU_DELETE_T(renderer, ParticleRenderer, MEMCATEGORY_SCENE_OBJECTS);
			}
		}

	protected:
		/**
		*/
		template<class T>
		ParticleRenderer * _createRenderer() {
			ParticleRenderer * particleRenderer = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
			particleRenderer->setRendererType(getRendererType());
			return particleRenderer;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_RENDERER_FACTORY_H__ */
