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

#ifndef __PU_DO_SCALE_EVENT_HANDLER_FACTORY_H__
#define __PU_DO_SCALE_EVENT_HANDLER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandlerFactory.h"
#include "ParticleUniverseDoScaleEventHandlerTokens.h"
#include "ParticleUniverseDoScaleEventHandler.h"

namespace ParticleUniverse
{
	/** This factory class is responsible for creation of a DoScaleEventHandler.
    */
	class _ParticleUniverseExport DoScaleEventHandlerFactory : public ParticleEventHandlerFactory
	{
		public:
			DoScaleEventHandlerFactory(void) {};
	        virtual ~DoScaleEventHandlerFactory(void) {};

			/** See ParticleEventHandlerFactory */
			virtual String getEventHandlerType(void) const
			{
				return "DoScale";
			}

			/** See ParticleEventHandlerFactory */
			virtual ParticleEventHandler* createEventHandler(void)
			{
				return _createEventHandler<DoScaleEventHandler>();
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mDoScaleEventHandlerTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mDoScaleEventHandlerTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate
				mDoScaleEventHandlerWriter.write(serializer, element);
			}

		protected:
			DoScaleEventHandlerWriter mDoScaleEventHandlerWriter;
			DoScaleEventHandlerTranslator mDoScaleEventHandlerTranslator;
	};

}
#endif
