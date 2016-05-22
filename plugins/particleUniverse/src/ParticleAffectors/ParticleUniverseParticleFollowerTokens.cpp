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

#include "ParticleAffectors/ParticleUniverseParticleFollowerTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "Externs/ParticleUniverseGravityExtern.h"

#include "ParticleAffectors/ParticleUniverseParticleFollower.h"

namespace ParticleUniverse {
	
	bool ParticleFollowerTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		ParticleFollower * affector = static_cast<ParticleFollower *>(af);

		if (prop->name == token[TOKEN_MIN_DISTANCE]) {
			// Property: min_distance
			if (passValidateProperty(compiler, prop, token[TOKEN_MIN_DISTANCE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMinDistance(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_FOLLOW_MIN_DISTANCE]) {
			// Property: follower_min_distance (deprecated and replaced by 'min_distance')
			if (passValidateProperty(compiler, prop, token[TOKEN_FOLLOW_MIN_DISTANCE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMinDistance(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_DISTANCE]) {
			// Property: max_distance
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DISTANCE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDistance(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_FOLLOW_MAX_DISTANCE]) {
			// Property: follower_max_distance (deprecated and replaced by 'TOKEN_MAX_DISTANCE')
			if (passValidateProperty(compiler, prop, token[TOKEN_FOLLOW_MAX_DISTANCE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDistance(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool ParticleFollowerTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void ParticleFollowerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a ParticleFollower
		const ParticleFollower * affector = static_cast<const ParticleFollower *>(element);

		// Write the header of the ParticleFollower
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getMinDistance(), ParticleFollower::DEFAULT_MIN_DISTANCE)) {
			serializer->writeLine(token[TOKEN_MIN_DISTANCE], StringConverter::toString(affector->getMinDistance()), 12);
		}
		if (!almostEquals(affector->getMaxDistance(), ParticleFollower::DEFAULT_MAX_DISTANCE)) {
			serializer->writeLine(token[TOKEN_MAX_DISTANCE], StringConverter::toString(affector->getMaxDistance()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
