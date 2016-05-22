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

#include "Externs/ParticleUniverseGravityExternTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "Externs/ParticleUniverseGravityExtern.h"

namespace ParticleUniverse {

	bool GravityExternTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		Extern * ex = any_cast<Extern *>(prop->parent->context);
		GravityExtern * externObject = static_cast<GravityExtern *>(ex);

		if (prop->name == token[TOKEN_GRAVITY]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_GRAVITY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					externObject->setGravity(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_DISTANCE_THRESHOLD]) {
			// Property: distance_threshold
			if (passValidateProperty(compiler, prop, token[TOKEN_DISTANCE_THRESHOLD], VAL_REAL)) {
				Real val;
				if (getReal(prop->values.front(), &val)) {
					externObject->setDistanceThreshold(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_EXTERN_DISTANCE_THRESHOLD]) {
			// Property: attachable_distance_threshold (deprecated and replaced by 'distance_threshold')
			if (passValidateProperty(compiler, prop, token[TOKEN_EXTERN_DISTANCE_THRESHOLD], VAL_REAL)) {
				Real val;
				if (getReal(prop->values.front(), &val)) {
					externObject->setDistanceThreshold(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool GravityExternTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void GravityExternWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a GravityExtern
		const Extern * externObject = static_cast<const Extern *>(element);
		const GravityExtern * gravityExtern = static_cast<const GravityExtern *>(externObject);
		serializer->writeLine(token[TOKEN_EXTERN], externObject->getExternType(), externObject->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ExternWriter::write(serializer, element);
		AttachableWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(gravityExtern->getGravity(), GravityAffector::DEFAULT_GRAVITY)) {
			serializer->writeLine(token[TOKEN_GRAVITY], StringConverter::toString(gravityExtern->getGravity()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
