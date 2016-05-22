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

#include "ParticleAffectors/ParticleUniverseBaseForceAffectorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseBaseForceAffector.h"

namespace ParticleUniverse {
	
	bool BaseForceAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		BaseForceAffector * affector = static_cast<BaseForceAffector *>(af); // Do not check type; assume the type is correct.

		if (prop->name == token[TOKEN_FORCE_VECTOR]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_VECTOR], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setForceVector(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_FORCE_AFF_VECTOR]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFF_VECTOR], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setForceVector(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_FORCE_APPLICATION]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_APPLICATION], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_FORCE_ADD]) {
						affector->setForceApplication(BaseForceAffector::FA_ADD);
						return true;
					} else if (val == token[TOKEN_FORCE_AVERAGE]) {
						affector->setForceApplication(BaseForceAffector::FA_AVERAGE);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_FORCE_AFF_APPLICATION]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFF_APPLICATION], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_FORCE_ADD]) {
						affector->setForceApplication(BaseForceAffector::FA_ADD);
						return true;
					} else if (val == token[TOKEN_FORCE_AVERAGE]) {
						affector->setForceApplication(BaseForceAffector::FA_AVERAGE);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool BaseForceAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void BaseForceAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a BaseForceAffector
		const BaseForceAffector * affector = static_cast<const BaseForceAffector *>(element);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->getForceVector() != BaseForceAffector::DEFAULT_FORCE_VECTOR) {
			serializer->writeLine(token[TOKEN_FORCE_VECTOR], StringConverter::toString(affector->getForceVector()), 12);
		}

		if (affector->getForceApplication() != BaseForceAffector::DEFAULT_FORCE_APPL) {
			String application = token[TOKEN_FORCE_ADD];
			if (affector->getForceApplication() == BaseForceAffector::FA_AVERAGE) {
				application = token[TOKEN_FORCE_AVERAGE];
			}
			serializer->writeLine(token[TOKEN_FORCE_APPLICATION], application, 12);
		}
	}

} /* namespace ParticleUniverse */
