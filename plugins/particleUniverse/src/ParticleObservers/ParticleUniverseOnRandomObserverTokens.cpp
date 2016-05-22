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

#include "ParticleObservers/ParticleUniverseOnRandomObserverTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

namespace ParticleUniverse {
	
	bool OnRandomObserverTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleObserver * ob = any_cast<ParticleObserver *>(prop->parent->context);
		OnRandomObserver * observer = static_cast<OnRandomObserver *>(ob);

		if (prop->name == token[TOKEN_ONRANDOM_THRESHOLD]) {
			// Property: random_threshold
			if (passValidateProperty(compiler, prop, token[TOKEN_ONRANDOM_THRESHOLD], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					observer->setThreshold(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool OnRandomObserverTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void OnRandomObserverWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a OnRandomObserver
		const OnRandomObserver * observer = static_cast<const OnRandomObserver *>(element);

		// Write the header of the OnRandomObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(observer->getThreshold(), OnRandomObserver::DEFAULT_THRESHOLD)) {
			serializer->writeLine(token[TOKEN_ONRANDOM_THRESHOLD], StringConverter::toString(observer->getThreshold()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
