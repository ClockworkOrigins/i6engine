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

#include "ParticleObservers/ParticleUniverseOnCountObserverTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"

namespace ParticleUniverse {
	
	bool OnCountObserverTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleObserver * ob = any_cast<ParticleObserver *>(prop->parent->context);
		OnCountObserver * observer = static_cast<OnCountObserver *>(ob);

		if (prop->name == token[TOKEN_ONCOUNT_THRESHOLD]) {
			// Property: count_threshold
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONCOUNT_THRESHOLD], 2)) {
				String compareType;
				uint val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if (getString(*i, &compareType)) {
					if (compareType == token[TOKEN_LESS_THAN]) {
						observer->setCompare(CO_LESS_THAN);
					} else if (compareType == token[TOKEN_GREATER_THAN]) {
						observer->setCompare(CO_GREATER_THAN);
					} else if (compareType == token[TOKEN_EQUALS]) {
						observer->setCompare(CO_EQUALS);
					}
					++i;
					if (getUInt(*i, &val)) {
						observer->setThreshold(val);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool OnCountObserverTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void OnCountObserverWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a OnCountObserver
		const OnCountObserver * observer = static_cast<const OnCountObserver *>(element);

		// Write the header of the OnCountObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		String compare = token[TOKEN_GREATER_THAN];
		if (observer->getCompare() == CO_LESS_THAN) {
			compare = token[TOKEN_LESS_THAN];
		} else if (observer->getCompare() == CO_EQUALS) {
			compare = token[TOKEN_EQUALS];
		}
		serializer->writeLine(token[TOKEN_ONCOUNT_THRESHOLD], compare, StringConverter::toString(observer->getThreshold()), 12);

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
