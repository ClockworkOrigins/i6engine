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

#ifndef __PU_BASE_ATTRACTOR_FACTORY_H__
#define __PU_BASE_ATTRACTOR_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseBaseForceAffectorTokens.h"
#include "ParticleUniverseBaseForceAffector.h"
#include "ParticleUniverseAffectorFactory.h"

namespace ParticleUniverse
{
	/** This factory is just a dummy factory.
    */
	class _ParticleUniverseExport BaseForceAffectorFactory : public ParticleAffectorFactory
	{
		protected:
			BaseForceAffectorWriter mBaseForceAffectorWriter;
			BaseForceAffectorTranslator mBaseForceAffectorTranslator;

		public:
			BaseForceAffectorFactory(void) {};
	        virtual ~BaseForceAffectorFactory(void) {};

			/** See ParticleAffectorFactory */
			String getAffectorType(void) const
			{
				return "Dummy01"; // Dummy Factory, only for setting up token definitions.
			}

			/** See ParticleAffectorFactory  */
			ParticleAffector* createAffector(void)
			{
				EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "PU: BaseForceAffectorFactory is a dummy factory.", 
					"BaseForceAffectorFactory::createAffector");
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBaseForceAffectorTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mBaseForceAffectorTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate
				mBaseForceAffectorWriter.write(serializer, element);
			}
	};

}
#endif
