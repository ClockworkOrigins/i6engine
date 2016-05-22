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

#include "ParticleAffectors/ParticleUniversePathFollowerTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniversePathFollower.h"

namespace ParticleUniverse {
	
	bool PathFollowerTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		PathFollower * affector = static_cast<PathFollower *>(af);

		if (prop->name == token[TOKEN_PATH_POINT]) {
			// Property: path_follower_point
			if (passValidateProperty(compiler, prop, token[TOKEN_PATH_POINT], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					affector->addPoint(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool PathFollowerTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void PathFollowerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a PathFollower
		const PathFollower * affector = static_cast<const PathFollower *>(element);

		// Write the header of the PathFollower
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		unsigned short numberOfPoints = affector->getNumPoints();
		if (numberOfPoints > 0) {
			for (unsigned short u = 0; u < numberOfPoints; ++u)
			{
				serializer->writeLine(token[TOKEN_PATH_POINT], StringConverter::toString(affector->getPoint(u)), 12);
			}
		}
				
		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
