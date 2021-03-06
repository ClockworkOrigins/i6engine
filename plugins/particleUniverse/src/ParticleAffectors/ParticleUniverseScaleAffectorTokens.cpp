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

#include "ParticleAffectors/ParticleUniverseScaleAffectorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseScaleAffector.h"

namespace ParticleUniverse {
	
	bool ScaleAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		ScaleAffector * affector = static_cast<ScaleAffector *>(af);

		if (prop->name == token[TOKEN_SCALE_XYZ_SCALE]) {
			// Property: xyz_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_XYZ_SCALE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleXYZ(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SCALE_X_SCALE]) {
			// Property: x_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_X_SCALE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleX(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SCALE_Y_SCALE]) {
			// Property: y_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_Y_SCALE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleY(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SCALE_Z_SCALE]) {
			// Property: z_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_Z_SCALE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleZ(dynamicAttributeFixed);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SINCE_START_SYSTEM]) {
			// Property: since_start_system
			if (passValidateProperty(compiler, prop, token[TOKEN_SINCE_START_SYSTEM], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setSinceStartSystem(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool ScaleAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * child = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(child->parent->context);
		ScaleAffector * affector = static_cast<ScaleAffector *>(af);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_SCALE_XYZ_SCALE]) {
			// Property: xyz_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setDynScaleXYZ(dynamicAttribute);
			return true;
		} else if (child->cls == token[TOKEN_SCALE_X_SCALE]) {
			// Property: x_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setDynScaleX(dynamicAttribute);
			return true;
		} else if (child->cls == token[TOKEN_SCALE_Y_SCALE]) {
			// Property: y_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setDynScaleY(dynamicAttribute);
			return true;
		} else if (child->cls == token[TOKEN_SCALE_Z_SCALE]) {
			// Property: z_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setDynScaleZ(dynamicAttribute);
			return true;
		}

		return false;
	}
	
	void ScaleAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement* element) {
		// Cast the element to a ScaleAffector
		const ScaleAffector * affector = static_cast<const ScaleAffector *>(element);

		// Write the header of the ScaleAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		DynamicAttributeFactory dynamicAttributeFactory;
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getDynScaleXYZ()), ScaleAffector::DEFAULT_XYZ_SCALE)) {
			serializer->setKeyword(token[TOKEN_SCALE_XYZ_SCALE]);
			serializer->setIndentation(12);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleXYZ());
		}
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getDynScaleX()), ScaleAffector::DEFAULT_X_SCALE)) {
			serializer->setKeyword(token[TOKEN_SCALE_X_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleX());
		}
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getDynScaleY()), ScaleAffector::DEFAULT_Y_SCALE)) {
			serializer->setKeyword(token[TOKEN_SCALE_Y_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleY());
		}
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getDynScaleZ()), ScaleAffector::DEFAULT_Z_SCALE)) {
			serializer->setKeyword(token[TOKEN_SCALE_Z_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleZ());
		}
		if (affector->isSinceStartSystem()) {
			serializer->writeLine(token[TOKEN_SINCE_START_SYSTEM], StringConverter::toString(affector->isSinceStartSystem()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
