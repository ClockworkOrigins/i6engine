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

#include "ParticleEventHandlers/ParticleUniverseDoExpireEventHandlerTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEventHandlers/ParticleUniverseDoExpireEventHandler.h"

namespace ParticleUniverse {
	
	void DoExpireEventHandlerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a DoExpireEventHandler
		const DoExpireEventHandler * eventHandler = static_cast<const DoExpireEventHandler *>(element);

		// Write the header of the DoExpireEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

} /* namespace ParticleUniverse */
