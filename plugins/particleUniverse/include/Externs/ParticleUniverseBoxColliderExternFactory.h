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

#ifndef __PU_BOX_COLLIDER_EXTERN_FACTORY_H__
#define __PU_BOX_COLLIDER_EXTERN_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExternFactory.h"
#include "ParticleUniverseBoxColliderExtern.h"
#include "ParticleUniverseBoxColliderExternTokens.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating the BoxColliderExtern.
	*/
	class _ParticleUniverseExport BoxColliderExternFactory : public ExternFactory
	{
		public:
			BoxColliderExternFactory(void) : 
				ExternFactory(){};
		     virtual ~BoxColliderExternFactory(void){};

			/** 
			*/
			String getExternType(void) const
			{
				return "BoxCollider";
			};

			/** 
			*/
			Extern* createExtern(void)
			{
				return _createExtern<BoxColliderExtern>();
			};

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBoxColliderExternTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBoxColliderExternTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate to mBoxColliderExternWriter
				mBoxColliderExternWriter.write(serializer, element);
			}

		protected:
			BoxColliderExternWriter mBoxColliderExternWriter;
			BoxColliderExternTranslator mBoxColliderExternTranslator;
	};

}
#endif
