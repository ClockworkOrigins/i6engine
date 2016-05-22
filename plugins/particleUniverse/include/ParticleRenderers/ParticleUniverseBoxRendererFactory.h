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

#ifndef __PU_BOX_RENDERER_FACTORY_H__
#define __PU_BOX_RENDERER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseBoxRenderer.h"
#include "ParticleUniverseBoxRendererTokens.h"
#include "ParticleUniverseRendererFactory.h"

namespace ParticleUniverse {

	/** Factory class responsible for creating a BoxRenderer.
	 */
	class _ParticleUniverseExport BoxRendererFactory : public ParticleRendererFactory {
	public:
		BoxRendererFactory() : mBoxRendererWriter() {}
		virtual ~BoxRendererFactory() {}

		/** See ParticleRendererFactory */
		String getRendererType() const {
			return "Box";
		}

		/** See ParticleRendererFactory */
		ParticleRenderer * createRenderer() {
			return _createRenderer<BoxRenderer>();
		}

		/*  */
		virtual void write(ParticleScriptSerializer * serializer, const IElement * element) {
			// Delegate
			mBoxRendererWriter.write(serializer, element);
		}

	protected:
		BoxRendererWriter mBoxRendererWriter;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_BOX_RENDERER_FACTORY_H__ */
