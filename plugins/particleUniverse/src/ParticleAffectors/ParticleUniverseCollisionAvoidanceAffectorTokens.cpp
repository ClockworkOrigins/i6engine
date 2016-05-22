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

#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffectorTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseCollisionAvoidanceAffector.h"

namespace ParticleUniverse {
	
	bool CollisionAvoidanceAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		CollisionAvoidanceAffector * affector = static_cast<CollisionAvoidanceAffector *>(af);

		if (prop->name == token[TOKEN_AVOIDANCE_RADIUS]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_AVOIDANCE_RADIUS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setRadius(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool CollisionAvoidanceAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void CollisionAvoidanceAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a CollisionAvoidanceAffector
		const CollisionAvoidanceAffector * affector = static_cast<const CollisionAvoidanceAffector *>(element);

		// Write the header of the CollisionAvoidanceAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getRadius(), CollisionAvoidanceAffector::DEFAULT_RADIUS)) {
			serializer->writeLine(token[TOKEN_AVOIDANCE_RADIUS], StringConverter::toString(affector->getRadius()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
