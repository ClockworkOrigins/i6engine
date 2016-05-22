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

#include "ParticleAffectors/ParticleUniversePlaneColliderTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"

namespace ParticleUniverse {
	
	bool PlaneColliderTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		PlaneCollider * affector = static_cast<PlaneCollider *>(af);

		if (prop->name == token[TOKEN_NORMAL]) {
			// Property: normal
			if (passValidateProperty(compiler, prop, token[TOKEN_NORMAL], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setNormal(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_PLANECOLL_NORMAL]) {
			// Property: plane_collider_normal (deprecated and replaced by 'normal')
			if (passValidateProperty(compiler, prop, token[TOKEN_PLANECOLL_NORMAL], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setNormal(val);
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
	
	bool PlaneColliderTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void PlaneColliderWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a PlaneCollider
		const PlaneCollider * affector = static_cast<const PlaneCollider *>(element);

		// Write the header of the PlaneCollider
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		BaseColliderWriter::write(serializer, element);

		// Write own attributes
		if (affector->getNormal() != PlaneCollider::DEFAULT_NORMAL) {
			serializer->writeLine(token[TOKEN_NORMAL], StringConverter::toString(affector->getNormal()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
