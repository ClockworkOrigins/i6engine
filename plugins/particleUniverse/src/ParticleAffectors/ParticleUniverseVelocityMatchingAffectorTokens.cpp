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

#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffectorTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseVelocityMatchingAffector.h"

namespace ParticleUniverse {
	
	bool VelocityMatchingAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		VelocityMatchingAffector * affector = static_cast<VelocityMatchingAffector *>(af);

		if (prop->name == token[TOKEN_RADIUS]) {
			// Property: radius
			if (passValidateProperty(compiler, prop, token[TOKEN_RADIUS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setRadius(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VELO_MATCHING_RADIUS]) {
			// Property: velocity_matching_radius (deprecated and replaced by radius)
			if (passValidateProperty(compiler, prop, token[TOKEN_VELO_MATCHING_RADIUS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setRadius(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool VelocityMatchingAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void VelocityMatchingAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a VelocityMatchingAffector
		const VelocityMatchingAffector * affector = static_cast<const VelocityMatchingAffector *>(element);

		// Write the header of the VelocityMatchingAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getRadius(), VelocityMatchingAffector::DEFAULT_RADIUS)) {
			serializer->writeLine(token[TOKEN_RADIUS], StringConverter::toString(affector->getRadius()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
