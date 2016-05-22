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

#ifndef __PU_SCENE_DECORATOR_FACTORY_H__
#define __PU_SCENE_DECORATOR_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseExternFactory.h"
#include "ParticleUniverseSceneDecoratorExtern.h"
#include "ParticleUniverseSceneDecoratorExternTokens.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating the SceneDecoratorExtern.
    */
	class _ParticleUniverseExport SceneDecoratorExternFactory : public ExternFactory
	{
		public:
			SceneDecoratorExternFactory(void) : 
				ExternFactory(){};
	        virtual ~SceneDecoratorExternFactory(void){};

			/** 
			*/
			String getExternType(void) const
			{
				return "SceneDecorator";
			};

			/** 
			*/
			Extern* createExtern(void)
			{
				return _createExtern<SceneDecoratorExtern>();
			};

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSceneDecoratorExternTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mSceneDecoratorExternTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate to mSceneDecoratorExternWriter
				mSceneDecoratorExternWriter.write(serializer, element);
			}

		protected:
			SceneDecoratorExternWriter mSceneDecoratorExternWriter;
			SceneDecoratorExternTranslator mSceneDecoratorExternTranslator;
	};

}
#endif
