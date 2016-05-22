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

#include "ParticleAffectors/ParticleUniverseBoxColliderTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseBoxCollider.h"

namespace ParticleUniverse {
	
	bool BoxColliderTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		BoxCollider * affector = static_cast<BoxCollider *>(af);
		if (prop->name == token[TOKEN_BOX_WIDTH]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_WIDTH], VAL_REAL)) {
				// Property: box_width
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setWidth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOXCOLL_WIDTH]) {
			// Property: box_collider_width (deprecated and replaced by 'box_width')
			if (passValidateProperty(compiler, prop, token[TOKEN_BOXCOLL_WIDTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setWidth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOX_HEIGHT]) {
			// Property: box_height
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_HEIGHT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setHeight(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOXCOLL_HEIGHT]) {
			// Property: box_collider_height (deprecated and replaced by 'box_height')
			if (passValidateProperty(compiler, prop, token[TOKEN_BOXCOLL_HEIGHT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setHeight(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOX_DEPTH]) {
			// Property: box_depth
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_DEPTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setDepth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOXCOLL_DEPTH]) {
			// Property: box_collider_depth (deprecated and replaced by 'box_depth')
			if (passValidateProperty(compiler, prop, token[TOKEN_BOXCOLL_DEPTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setDepth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_INNER_COLLISION]) {
			// Property: inner_collision
			if (passValidateProperty(compiler, prop, token[TOKEN_INNER_COLLISION], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setInnerCollision(val);
					return true;
				}
			}
		} else {
			// Parse the BaseCollider
			BaseColliderTranslator baseColliderTranslator;
			return baseColliderTranslator.translateChildProperty(compiler, node);
		}

		return false;
	}
	
	bool BoxColliderTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void BoxColliderWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a BoxCollider
		const BoxCollider * affector = static_cast<const BoxCollider *>(element);

		// Write the header of the BoxCollider
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		BaseColliderWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getWidth(), BoxCollider::DEFAULT_WIDTH)) {
			serializer->writeLine(token[TOKEN_BOX_WIDTH], StringConverter::toString(affector->getWidth()), 12);
		}
		if (!almostEquals(affector->getHeight(), BoxCollider::DEFAULT_HEIGHT)) {
			serializer->writeLine(token[TOKEN_BOX_HEIGHT], StringConverter::toString(affector->getHeight()), 12);
		}
		if (!almostEquals(affector->getDepth(), BoxCollider::DEFAULT_DEPTH)) {
			serializer->writeLine(token[TOKEN_BOX_DEPTH], StringConverter::toString(affector->getDepth()), 12);
		}
		if (affector->isInnerCollision() != false) {
			serializer->writeLine(token[TOKEN_INNER_COLLISION], StringConverter::toString(affector->isInnerCollision()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
