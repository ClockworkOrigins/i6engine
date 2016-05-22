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

#include "ParticleAffectors/ParticleUniverseScaleVelocityAffectorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseScaleVelocityAffector.h"

namespace ParticleUniverse {
	
	bool ScaleVelocityAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		ScaleVelocityAffector * affector = static_cast<ScaleVelocityAffector *>(af);

		if (prop->name == token[TOKEN_SCALE_VELOCITY_SCALE]) {
			// Property: velocity_scale
			if (passValidateProperty(compiler, prop, token[TOKEN_SCALE_VELOCITY_SCALE], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					DynamicAttributeFixed * dynamicAttributeFixed = PU_NEW_T(DynamicAttributeFixed, MEMCATEGORY_SCENE_OBJECTS)();
					dynamicAttributeFixed->setValue(val);
					affector->setDynScaleVelocity(dynamicAttributeFixed);
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
		} else if (prop->name == token[TOKEN_STOP_AT_FLIP]) {
			// Property: stop_at_flip
			if (passValidateProperty(compiler, prop, token[TOKEN_STOP_AT_FLIP], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setStopAtFlip(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool ScaleVelocityAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * child = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(child->parent->context);
		ScaleVelocityAffector * affector = static_cast<ScaleVelocityAffector *>(af);

		DynamicAttributeTranslator dynamicAttributeTranslator;
		if (child->cls == token[TOKEN_SCALE_VELOCITY_SCALE]) {
			// Property: velocity_scale
			dynamicAttributeTranslator.translate(compiler, node);
			DynamicAttribute * dynamicAttribute = any_cast<DynamicAttribute *>(child->context);
			affector->setDynScaleVelocity(dynamicAttribute);
			return true;
		}

		return false;
	}
	
	void ScaleVelocityAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a ScaleVelocityAffector
		const ScaleVelocityAffector * affector = static_cast<const ScaleVelocityAffector *>(element);

		// Write the header of the ScaleVelocityAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		DynamicAttributeFactory dynamicAttributeFactory;
		if (!almostEquals(dynamicAttributeFactory._getDefaultValue(affector->getDynScaleVelocity()), ScaleVelocityAffector::DEFAULT_VELOCITY_SCALE)) {
			serializer->setKeyword(token[TOKEN_SCALE_VELOCITY_SCALE]);
			dynamicAttributeFactory.write(serializer, affector->getDynScaleVelocity());
		}
		if (affector->isSinceStartSystem()) {
			serializer->writeLine(token[TOKEN_SINCE_START_SYSTEM], StringConverter::toString(affector->isSinceStartSystem()), 12);
		}

		if (affector->isStopAtFlip()) {
			serializer->writeLine(token[TOKEN_STOP_AT_FLIP], StringConverter::toString(affector->isStopAtFlip()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
