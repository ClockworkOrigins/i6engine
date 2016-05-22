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

#ifndef __PU_BASE_COLLIDER_FACTORY_H__
#define __PU_BASE_COLLIDER_FACTORY_H__

#include "ParticleUniverseAffectorFactory.h"
#include "ParticleUniverseBaseColliderTokens.h"
#include "ParticleUniverseException.h"

namespace ParticleUniverse
{
	/** This factory is just a dummy factory.
    */
	class _ParticleUniverseExport BaseColliderFactory : public ParticleAffectorFactory
	{
		protected:
			BaseColliderWriter mBaseColliderWriter;
			BaseColliderTranslator mBaseColliderTranslator;

		public:
			BaseColliderFactory(void) {}
	        virtual ~BaseColliderFactory(void) {}

			/** See ParticleAffectorFactory */
			String getAffectorType(void) const
			{
				return "Dummy02"; // Dummy Factory, only for setting up token definitions.
			}

			/** See ParticleAffectorFactory  */
			ParticleAffector* createAffector(void)
			{
				EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "PU: BaseColliderFactory is a dummy factory.", 
					"BaseColliderFactory::createAffector");
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBaseColliderTranslator.translateChildProperty(compiler, node);
			}

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBaseColliderTranslator.translateChildObject(compiler, node);
			}

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate
				mBaseColliderWriter.write(serializer, element);
			}
	};

}
#endif
