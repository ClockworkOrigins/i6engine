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

#include "ParticleEmitters/ParticleUniversePositionEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniversePositionEmitter.h"

namespace ParticleUniverse {
	
	bool PositionEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		PositionEmitter * emitter = static_cast<PositionEmitter *>(em);

		if (prop->name == token[TOKEN_POS_ADD_POSITION]) {
			// Property: add_position
			if (passValidateProperty(compiler, prop, token[TOKEN_POS_ADD_POSITION], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					emitter->addPosition(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_POS_RANDOMIZE]) {
			// Property: random_position
			if (passValidateProperty(compiler, prop, token[TOKEN_POS_RANDOMIZE], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					emitter->setRandomized(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool PositionEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void PositionEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a PositionEmitter
		const PositionEmitter * emitter = static_cast<const PositionEmitter *>(element);

		// Write the header of the PositionEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->isRandomized() != PositionEmitter::DEFAULT_RANDOMIZE) {
			serializer->writeLine(token[TOKEN_POS_RANDOMIZE], StringConverter::toString(emitter->isRandomized()), 12);
		}

		// List of positions
		vector<Vector3> positionList = emitter->getPositions();
		if (!positionList.empty()) {
			for (vector<Vector3>::const_iterator it = positionList.begin(); it != positionList.end(); ++it) {
				serializer->writeLine(token[TOKEN_POS_ADD_POSITION], StringConverter::toString(*it), 12);
			}
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
