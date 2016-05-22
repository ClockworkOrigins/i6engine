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

#include "ParticleAffectors/ParticleUniverseBaseColliderTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseBaseCollider.h"

namespace ParticleUniverse {
	
	bool BaseColliderTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		BaseCollider * affector = static_cast<BaseCollider *>(af); // Do not check type; assume the type is correct.

		if (prop->name == token[TOKEN_FRICTION]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_FRICTION], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFriction(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_COLLIDER_FRICTION]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_FRICTION], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFriction(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOUNCYNESS]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_BOUNCYNESS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setBouncyness(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_COLLIDER_BOUNCYNESS]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_BOUNCYNESS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setBouncyness(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_INTERSECTION] || prop->name == token[TOKEN_COLLIDER_INTERSECTION]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_INTERSECTION], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_COLLIDER_INTERSECTION_POINT]) {
						affector->setIntersectionType(BaseCollider::IT_POINT);
						return true;
					} else if (val == token[TOKEN_COLLIDER_INTERSECTION_BOX]) {
						affector->setIntersectionType(BaseCollider::IT_BOX);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_COLLIDER_COLLISION_TYPE]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLIDER_COLLISION_TYPE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_COLLIDER_BOUNCE]) {
						affector->setCollisionType(BaseCollider::CT_BOUNCE);
						return true;
					} else if (val == token[TOKEN_COLLIDER_FLOW]) {
						affector->setCollisionType(BaseCollider::CT_FLOW);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool BaseColliderTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void BaseColliderWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a BaseCollider
		const BaseCollider * affector = static_cast<const BaseCollider *>(element);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getFriction(), BaseCollider::DEFAULT_FRICTION)) {
			serializer->writeLine(token[TOKEN_FRICTION], StringConverter::toString(affector->getFriction()), 12);
		}
		if (!almostEquals(affector->getBouncyness(), BaseCollider::DEFAULT_BOUNCYNESS)) {
			serializer->writeLine(token[TOKEN_BOUNCYNESS], StringConverter::toString(affector->getBouncyness()), 12);
		}
		if (affector->getIntersectionType() != BaseCollider::DEFAULT_INTERSECTION_TYPE) {
			String intersectionType = token[TOKEN_COLLIDER_INTERSECTION_POINT];
			if (affector->getIntersectionType() == BaseCollider::IT_BOX) {
				intersectionType = token[TOKEN_COLLIDER_INTERSECTION_BOX];
			}
			serializer->writeLine(token[TOKEN_INTERSECTION], intersectionType, 12);
		}
		if (affector->getCollisionType() != BaseCollider::DEFAULT_COLLISION_TYPE) {
			String collisionType = token[TOKEN_COLLIDER_BOUNCE];
			if (affector->getCollisionType() == BaseCollider::CT_FLOW) {
				collisionType = token[TOKEN_COLLIDER_FLOW];
			} else if (affector->getCollisionType() == BaseCollider::CT_NONE) {
				collisionType = token[TOKEN_COLLIDER_NONE];
			}
			serializer->writeLine(token[TOKEN_COLLIDER_COLLISION_TYPE], collisionType, 12);
		}
	}

} /* namespace ParticleUniverse */
