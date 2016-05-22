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

#ifndef __PU_RIBBONTRAIL_RENDERER_FACTORY_H__
#define __PU_RIBBONTRAIL_RENDERER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRendererFactory.h"
#include "ParticleUniverseRibbonTrailRenderer.h"
#include "ParticleUniverseRibbonTrailRendererTokens.h"

namespace ParticleUniverse
{
	/** Factory class responsible for creating a RibbonTrailRenderer.
    */
	class _ParticleUniverseExport RibbonTrailRendererFactory : public ParticleRendererFactory
	{
		public:
			RibbonTrailRendererFactory(void) {};
	        virtual ~RibbonTrailRendererFactory(void) {};

			/** See ParticleRendererFactory */
			String getRendererType(void) const
			{
				return "RibbonTrail";
			}

			/** See ParticleRendererFactory */
			ParticleRenderer* createRenderer(void)
			{
				return _createRenderer<RibbonTrailRenderer>();
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mRibbonTrailRendererTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node)
			{
				return mRibbonTrailRendererTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate
				mRibbonTrailRendererWriter.write(serializer, element);
			}


		protected:
			RibbonTrailRendererWriter mRibbonTrailRendererWriter;
			RibbonTrailRendererTranslator mRibbonTrailRendererTranslator;
	};

}
#endif
