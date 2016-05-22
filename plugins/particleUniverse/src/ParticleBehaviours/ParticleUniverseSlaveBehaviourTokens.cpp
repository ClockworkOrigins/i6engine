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

#include "ParticleBehaviours/ParticleUniverseSlaveBehaviourTokens.h"

#include "ParticleUniverseScriptSerializer.h"
#include "ParticleBehaviours/ParticleUniverseSlaveBehaviour.h"

namespace ParticleUniverse {

	void SlaveBehaviourWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a SlaveBehaviour
		const SlaveBehaviour * behaviour = static_cast<const SlaveBehaviour *>(element);

		// Write the header of the SlaveBehaviour
		serializer->writeLine(token[TOKEN_BEHAVIOUR], behaviour->getBehaviourType(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleBehaviourWriter::write(serializer, element);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
