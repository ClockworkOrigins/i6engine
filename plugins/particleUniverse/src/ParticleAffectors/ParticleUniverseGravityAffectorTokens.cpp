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

#include "ParticleAffectors/ParticleUniverseGravityAffectorTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "Externs/ParticleUniverseGravityExtern.h"

#include "ParticleAffectors/ParticleUniverseGravityAffector.h"


namespace ParticleUniverse {
	
	bool GravityAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		GravityAffector * affector = static_cast<GravityAffector *>(af);

		if (prop->name == token[TOKEN_GRAVITY]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_GRAVITY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setGravity(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool GravityAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void GravityAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a GravityAffector
		const GravityAffector * affector = static_cast<const GravityAffector *>(element);

		// Write the header of the GravityAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getGravity(), GravityAffector::DEFAULT_GRAVITY)) {
			serializer->writeLine(token[TOKEN_GRAVITY], StringConverter::toString(affector->getGravity()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
