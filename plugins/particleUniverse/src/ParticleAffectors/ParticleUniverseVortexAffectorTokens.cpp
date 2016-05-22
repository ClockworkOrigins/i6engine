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

#include "ParticleAffectors/ParticleUniverseVortexAffectorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseVortexAffector.h"

namespace ParticleUniverse {
	
	bool VortexAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		VortexAffector * affector = static_cast<VortexAffector *>(af);

		if (prop->name == token[TOKEN_ROTATION_AXIS]) {
			// Property: rotation_axis
			if (passValidateProperty(compiler, prop, token[TOKEN_ROTATION_AXIS], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setRotationVector(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VORTEX_ROTATION_VECTOR]) {
			// Property: vortex_aff_vector (deprecated and replaced by 'rotation_axis')
			if (passValidateProperty(compiler, prop, token[TOKEN_VORTEX_ROTATION_VECTOR], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setRotationVector(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ROTATION_SPEED]) {
			// Property: rotation_speed
			if (passValidateProperty(compiler, prop, token[TOKEN_ROTATION_SPEED], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setRotationSpeed(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VORTEX_ROTATION_SPEED]) {
			// Property: vortex_aff_speed (deprecated and replaced by 'rotation_speed')
			if (passValidateProperty(compiler, prop, token[TOKEN_VORTEX_ROTATION_SPEED], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setRotationSpeed(dynamicAttributeFixed);
					return true;
				}
			}
		}

		return false;
	}
	
	bool VortexAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * child = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(child->parent->context);
		VortexAffector * affector = static_cast<VortexAffector *>(af);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_ROTATION_SPEED]) {
			// Property: rotation_speed
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setRotationSpeed(dynamicAttribute);
			return true;
		} else if (child->cls == token[TOKEN_VORTEX_ROTATION_SPEED]) {
			// Property: vortex_aff_speed (deprecated and replaced by 'rotation_speed')
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setRotationSpeed(dynamicAttribute);
			return true;
		}

		return false;
	}
	
	void VortexAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a VortexAffector
		const VortexAffector * affector = static_cast<const VortexAffector *>(element);

		// Write the header of the VortexAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->getRotationVector() != VortexAffector::DEFAULT_ROTATION_VECTOR) {
			serializer->writeLine(token[TOKEN_ROTATION_AXIS], StringConverter::toString(affector->getRotationVector()), 12);
		}
		DynamicAttributeFactory dynamicAttributeFactory;
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getRotationSpeed()), VortexAffector::DEFAULT_ROTATION_SPEED)) {
			serializer->setKeyword(token[TOKEN_ROTATION_SPEED]);
			serializer->setIndentation(12);
			dynamicAttributeFactory.write(serializer, affector->getRotationSpeed());
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
