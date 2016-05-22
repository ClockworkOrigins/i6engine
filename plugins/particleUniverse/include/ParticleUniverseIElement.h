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

#ifndef __PU_I_ELEMENT_H__
#define __PU_I_ELEMENT_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	/** The IElement is used to identify classes that must be set as part of a section in case a script is
		parsed.
	@remarks
		I.e. If the parses encounters a ´technique´ section in a particle universe script, a ParticleTechnique
		object is created and must be set in the current section of the Context object. the
		ParticleTechnique object must be of type IElement to be stored in the Context object.
	*/
	class _ParticleUniverseExport IElement {
	public:
		IElement() {}
		virtual ~IElement() {}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_I_ELEMENT_H__ */
