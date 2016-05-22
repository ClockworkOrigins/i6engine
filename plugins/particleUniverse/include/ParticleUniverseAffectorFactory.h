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

#ifndef __PU_AFFECTOR_FACTORY_H__
#define __PU_AFFECTOR_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffector.h"
#include "ParticleUniverseAffectorTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse
{
	/** This is the base factory of all ParticleAffector implementations.
    */
	class _ParticleUniverseExport ParticleAffectorFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc
	{
	    protected:

			/** 
	        */
			template <class T>
			ParticleAffector* _createAffector(void)
			{
				ParticleAffector* particleAffector = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
				particleAffector->setAffectorType(getAffectorType());
				return particleAffector;
			}

		public:
			ParticleAffectorFactory(void) {}
	        virtual ~ParticleAffectorFactory(void) {}

		    /** Returns the type of the factory, which identifies the particle affector type this factory creates. */
			virtual String getAffectorType(void) const = 0;

			/** Creates a new affector instance.
		    @remarks
	        */
		    virtual ParticleAffector* createAffector(void) = 0;

			/** Delete an affector
	        */
			void destroyAffector (ParticleAffector* affector)
			{
				PU_DELETE_T(affector, ParticleAffector, MEMCATEGORY_SCENE_OBJECTS);
			}
	};

}
#endif
