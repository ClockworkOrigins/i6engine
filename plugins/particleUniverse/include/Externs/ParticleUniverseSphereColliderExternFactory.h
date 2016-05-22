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

#ifndef __PU_SPHERE_COLLIDER_EXTERN_FACTORY_H__
#define __PU_SPHERE_COLLIDER_EXTERN_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExternFactory.h"
#include "ParticleUniverseSphereColliderExtern.h"
#include "ParticleUniverseSphereColliderExternTokens.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating the SphereColliderExtern.
    */
	class _ParticleUniverseExport SphereColliderExternFactory : public ExternFactory
	{
		public:
			SphereColliderExternFactory(void) : 
				ExternFactory(){};
	        virtual ~SphereColliderExternFactory(void){};

			/** 
			*/
			String getExternType(void) const
			{
				return "SphereCollider";
			};

			/** 
			*/
			Extern* createExtern(void)
			{
				return _createExtern<SphereColliderExtern>();
			};

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSphereColliderExternTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSphereColliderExternTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate to mSphereColliderExternWriter
				mSphereColliderExternWriter.write(serializer, element);
			}

		protected:
			SphereColliderExternWriter mSphereColliderExternWriter;
			SphereColliderExternTranslator mSphereColliderExternTranslator;
	};

}
#endif
