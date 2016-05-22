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

#ifndef __PU_BEHAVIOUR_FACTORY_H__
#define __PU_BEHAVIOUR_FACTORY_H__

#include "ParticleUniverseBehaviour.h"
#include "ParticleUniverseBehaviourTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all ParticleBehaviour implementations.
    */
	class _ParticleUniverseExport ParticleBehaviourFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
		ParticleBehaviourFactory() {}
		virtual ~ParticleBehaviourFactory() {}

		/** Returns the type of the factory, which identifies the particle behaviour type this factory creates. */
		virtual String getBehaviourType() const = 0;

		/** Creates a new behaviour instance.
		@remarks
	    */
		virtual ParticleBehaviour * createBehaviour() = 0;

		/** Delete a behaviour
	    */
		void destroyBehaviour(ParticleBehaviour * behaviour) {
			PU_DELETE_T(behaviour, ParticleBehaviour, MEMCATEGORY_SCENE_OBJECTS);
		}

	protected:
		/**
		*/
		template<class T>
		ParticleBehaviour * _createBehaviour() {
			ParticleBehaviour * particleBehaviour = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
			particleBehaviour->setBehaviourType(getBehaviourType());
			return particleBehaviour;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BEHAVIOUR_FACTORY_H__ */
