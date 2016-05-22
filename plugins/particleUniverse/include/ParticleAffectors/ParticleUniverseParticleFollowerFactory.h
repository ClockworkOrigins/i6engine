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

#ifndef __PU_PARTICLE_FOLLOWER_FACTORY_H__
#define __PU_PARTICLE_FOLLOWER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseParticleFollowerTokens.h"
#include "ParticleUniverseParticleFollower.h"
#include "ParticleUniverseAffectorFactory.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating the ParticleFollower.
    */
	class _ParticleUniverseExport ParticleFollowerFactory : public ParticleAffectorFactory
	{
		public:
			ParticleFollowerFactory(void){};
	        virtual ~ParticleFollowerFactory(void){};

			/** See ParticleAffectorFactory */
			String getAffectorType(void) const
			{
				return "ParticleFollower";
			}

			/** See ParticleAffectorFactory  */
			ParticleAffector* createAffector(void)
			{
				return _createAffector<ParticleFollower>();
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mParticleFollowerTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mParticleFollowerTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate
				mParticleFollowerWriter.write(serializer, element);
			}

		protected:
			ParticleFollowerWriter mParticleFollowerWriter;
			ParticleFollowerTranslator mParticleFollowerTranslator;
	};

}
#endif
