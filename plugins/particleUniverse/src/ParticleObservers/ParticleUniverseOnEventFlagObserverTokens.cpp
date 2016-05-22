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

#include "ParticleObservers/ParticleUniverseOnEventFlagObserverTokens.h"

#include "ParticleUniversePCH.h"

#ifndef PARTICLE_UNIVERSE_EXPORTS
#define PARTICLE_UNIVERSE_EXPORTS
#endif

#include "ParticleObservers/ParticleUniverseOnEventFlagObserver.h"

namespace ParticleUniverse {
	
	bool OnEventFlagObserverTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node)
	{
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleObserver * ob = any_cast<ParticleObserver *>(prop->parent->context);
		OnEventFlagObserver * observer = static_cast<OnEventFlagObserver *>(ob);

		if (prop->name == token[TOKEN_ONEVENT_FLAG]) {
			// Property: event_flag
			if (passValidateProperty(compiler, prop, token[TOKEN_ONEVENT_FLAG], VAL_UINT)) {
				uint32 val = 0;
				if (getUInt(prop->values.front(), &val)) {
					observer->setEventFlag(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool OnEventFlagObserverTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void OnEventFlagObserverWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a OnEventFlagObserver
		const OnEventFlagObserver * observer = static_cast<const OnEventFlagObserver *>(element);

		// Write the header of the OnEventFlagObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		if (observer->getEventFlag() != OnEventFlagObserver::DEFAULT_EVENT_FLAG) {
			serializer->writeLine(token[TOKEN_ONEVENT_FLAG], StringConverter::toString(observer->getEventFlag()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
