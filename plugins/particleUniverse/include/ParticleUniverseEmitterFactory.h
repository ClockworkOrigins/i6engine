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

#ifndef __PU_EMITTER_FACTORY_H__
#define __PU_EMITTER_FACTORY_H__

#include "ParticleUniverseEmitter.h"
#include "ParticleUniverseEmitterTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all ParticleEmitter implementations.
    */
	class _ParticleUniverseExport ParticleEmitterFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
			ParticleEmitterFactory() {}
	        virtual ~ParticleEmitterFactory() {}

			/** Returns the type of the factory, which identifies the particle emitter type this factory creates. */
			virtual String getEmitterType() const = 0;

	        /** Creates a new emitter instance.
		    @remarks
	        */
		    virtual ParticleEmitter * createEmitter() = 0;

			/** Delete an emitter 
	        */
			void destroyEmitter (ParticleEmitter * emitter) {
				PU_DELETE_T(emitter, ParticleEmitter, MEMCATEGORY_SCENE_OBJECTS);
			}

	protected:
		/**
		*/
		template<class T>
		ParticleEmitter * _createEmitter() {
			ParticleEmitter * particleEmitter = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
			particleEmitter->setEmitterType(getEmitterType());
			return particleEmitter;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EMITTER_FACTORY_H__ */
