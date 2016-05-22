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

#include "ParticleUniverseCameraDependencyTokens.h"

#include "ParticleUniverseAny.h"
#include "ParticleUniverseCameraDependency.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

namespace ParticleUniverse {

	/**************************************************************************
	 * CameraDependencyTranslator
	 *************************************************************************/
	CameraDependencyTranslator::CameraDependencyTranslator() {
	}

	void CameraDependencyTranslator::translate(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
		CameraDependency * cameraDependency = any_cast<CameraDependency *>(obj->context);

		// Run through properties
		for (AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i) {
			if ((*i)->type == ANT_PROPERTY) {
				PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>((*i).get());
				if (prop->name == token[TOKEN_DISTANCE_THRESHOLD]) {
					// Property: distance_threshold
					if (passValidateProperty(compiler, prop, token[TOKEN_DISTANCE_THRESHOLD], VAL_REAL)) {
						Real val;
						if (getReal(prop->values.front(), &val)) {
							cameraDependency->setThreshold(val);
						}
					}
				} else if (prop->name == token[TOKEN_INCREASE]) {
					// Property: increase
					if (passValidateProperty(compiler, prop, token[TOKEN_INCREASE], VAL_BOOL)) {
						bool val;
						if (getBoolean(prop->values.front(), &val)) {
							cameraDependency->setIncrease(val);
						}
					}
				} else {
					errorUnexpectedProperty(compiler, prop);
				}
			} else if((*i)->type == ANT_OBJECT) {
				processNode(compiler, *i);
			} else {
				errorUnexpectedToken(compiler, *i);
			}
		}
	}

	void CameraDependencyWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a CameraDependency
		const CameraDependency * dependency = static_cast<const CameraDependency *>(element);

		// Write attributes
		if (!almostEquals(dependency->getThreshold(), CameraDependency::DEFAULT_DISTANCE_THRESHOLD)) {
			serializer->writeLine(token[TOKEN_DISTANCE_THRESHOLD], StringConverter::toString(dependency->getThreshold()), 12);
		}
		if (!almostEquals(dependency->isIncrease(), CameraDependency::DEFAULT_INCREASE)) {
			serializer->writeLine(token[TOKEN_INCREASE], StringConverter::toString(dependency->isIncrease()), 12);
		}
	}

} /* namespace ParticleUniverse */
