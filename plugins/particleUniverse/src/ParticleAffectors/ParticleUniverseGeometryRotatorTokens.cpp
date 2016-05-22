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

#include "ParticleAffectors/ParticleUniverseGeometryRotatorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseGeometryRotator.h"

namespace ParticleUniverse {
	
	bool GeometryRotatorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		GeometryRotator * affector = static_cast<GeometryRotator *>(af);

		if (prop->name == token[TOKEN_USE_OWN_ROTATION]) {
			// Property: use_own_rotation
			if (passValidateProperty(compiler, prop, token[TOKEN_USE_OWN_ROTATION], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setUseOwnRotationSpeed(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_GEOMROT_USE_OWN_ROTATION]) {
			// Property: geom_rot_use_own_rotation (deprecated and replaced by 'use_own_rotation')
			if (passValidateProperty(compiler, prop, token[TOKEN_GEOMROT_USE_OWN_ROTATION], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setUseOwnRotationSpeed(val);
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
		} else if (prop->name == token[TOKEN_GEOMROT_ROTATION_SPEED]) {
			// Property: geom_rot_rotation_speed (deprecated and replaced by 'rotation_speed')
			if (passValidateProperty(compiler, prop, token[TOKEN_GEOMROT_ROTATION_SPEED], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setRotationSpeed(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ROTATION_AXIS]) {
			// Property: rotation_axis
			if (passValidateProperty(compiler, prop, token[TOKEN_ROTATION_AXIS], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setRotationAxis(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_GEOMROT_ROTATION_AXIS]) {
			// Property: geom_rot_axis (deprecated and replaced by 'rotation_axis')
			if (passValidateProperty(compiler, prop, token[TOKEN_GEOMROT_ROTATION_AXIS], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->setRotationAxis(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool GeometryRotatorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * child = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(child->parent->context);
		GeometryRotator * affector = static_cast<GeometryRotator *>(af);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_ROTATION_SPEED]) {
			// Property: rotation_speed
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setRotationSpeed(dynamicAttribute);
			return true;
		} else if (child->cls == token[TOKEN_GEOMROT_ROTATION_SPEED]) {
			// Property: geom_rot_rotation_speed (deprecated and replaced by 'rotation_speed')
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setRotationSpeed(dynamicAttribute);
			return true;
		}

		return false;
	}
	
	void GeometryRotatorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a GeometryRotator
		const GeometryRotator * affector = static_cast<const GeometryRotator *>(element);

		// Write the header of the GeometryRotator
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->useOwnRotationSpeed() != GeometryRotator::DEFAULT_USE_OWN) {
			serializer->writeLine(token[TOKEN_USE_OWN_ROTATION], StringConverter::toString(affector->useOwnRotationSpeed()), 12);
		}
		if (affector->getRotationAxis() != GeometryRotator::DEFAULT_ROTATION_AXIS) {
			serializer->writeLine(token[TOKEN_ROTATION_AXIS], StringConverter::toString(affector->getRotationAxis()), 12);
		}

		DynamicAttributeFactory dynamicAttributeFactory;
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getRotationSpeed()), GeometryRotator::DEFAULT_ROTATION_SPEED)) {
			serializer->setKeyword(token[TOKEN_ROTATION_SPEED]);
			serializer->setIndentation(12);
			dynamicAttributeFactory.write(serializer, affector->getRotationSpeed());
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
