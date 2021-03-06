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

#include "ParticleObservers/ParticleUniverseOnPositionObserverTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"

namespace ParticleUniverse {
	
	bool OnPositionObserverTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleObserver * ob = any_cast<ParticleObserver *>(prop->parent->context);
		OnPositionObserver * observer = static_cast<OnPositionObserver *>(ob);

		if (prop->name == token[TOKEN_ONPOSITION_X]) {
			// Property: position_x
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_X], 2)) {
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if (getString(*i, &compareType)) {
					if (compareType == token[TOKEN_LESS_THAN]) {
						observer->setComparePositionX(CO_LESS_THAN);
					} else if (compareType == token[TOKEN_GREATER_THAN]) {
						observer->setComparePositionX(CO_GREATER_THAN);
					} else if (compareType == token[TOKEN_EQUALS]) {
						observer->setComparePositionX(CO_EQUALS);
					}
					++i;
					if (getReal(*i, &val)) {
						observer->setPositionXThreshold(val);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_ONPOSITION_Y]) {
			// Property: position_y
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_Y], 2)) {
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if (getString(*i, &compareType)) {
					if (compareType == token[TOKEN_LESS_THAN]) {
						observer->setComparePositionY(CO_LESS_THAN);
					} else if (compareType == token[TOKEN_GREATER_THAN]) {
						observer->setComparePositionY(CO_GREATER_THAN);
					} else if (compareType == token[TOKEN_EQUALS]) {
						observer->setComparePositionY(CO_EQUALS);
					}
					++i;
					if (getReal(*i, &val)) {
						observer->setPositionYThreshold(val);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_ONPOSITION_Z]) {
			// Property: position_z
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_ONPOSITION_Z], 2)) {
				String compareType;
				Real val = 0;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if (getString(*i, &compareType)) {
					if (compareType == token[TOKEN_LESS_THAN]) {
						observer->setComparePositionZ(CO_LESS_THAN);
					} else if (compareType == token[TOKEN_GREATER_THAN]) {
						observer->setComparePositionZ(CO_GREATER_THAN);
					} else if (compareType == token[TOKEN_EQUALS]) {
						observer->setComparePositionZ(CO_EQUALS);
					}
					++i;
					if (getReal(*i, &val)) {
						observer->setPositionZThreshold(val);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool OnPositionObserverTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void OnPositionObserverWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a OnPositionObserver
		const OnPositionObserver * observer = static_cast<const OnPositionObserver *>(element);

		// Write the header of the OnPositionObserver
		serializer->writeLine(token[TOKEN_OBSERVER], observer->getObserverType(), observer->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleObserverWriter::write(serializer, element);

		// Write own attributes
		if (observer->isPositionXThresholdSet()) {
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionX() == CO_LESS_THAN) {
				compare = token[TOKEN_LESS_THAN];
			} else if (observer->getComparePositionX() == CO_EQUALS) {
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_X], compare, StringConverter::toString(observer->getPositionXThreshold()), 12);
		}
		if (observer->isPositionYThresholdSet()) {
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionY() == CO_LESS_THAN) {
				compare = token[TOKEN_LESS_THAN];
			} else if (observer->getComparePositionY() == CO_EQUALS) {
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_Y], compare, StringConverter::toString(observer->getPositionYThreshold()), 12);
		}
		if (observer->isPositionZThresholdSet()) {
			String compare = token[TOKEN_GREATER_THAN];
			if (observer->getComparePositionZ() == CO_LESS_THAN) {
				compare = token[TOKEN_LESS_THAN];
			} else if (observer->getComparePositionZ() == CO_EQUALS) {
				compare = token[TOKEN_EQUALS];
			}
			serializer->writeLine(token[TOKEN_ONPOSITION_Z], compare, StringConverter::toString(observer->getPositionZThreshold()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
