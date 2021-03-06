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

#include "ParticleEmitters/ParticleUniverseSlaveEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniverseSlaveEmitter.h"

namespace ParticleUniverse {
	
	bool SlaveEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		SlaveEmitter * emitter = static_cast<SlaveEmitter *>(em);

		if (prop->name == token[TOKEN_MASTER_TECHNIQUE]) {
			// Property: master_technique_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MASTER_TECHNIQUE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					emitter->setMasterTechniqueName(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MASTER_EMITTER]) {
			// Property: master_emitter_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MASTER_EMITTER], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					emitter->setMasterEmitterName(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool SlaveEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void SlaveEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a SlaveEmitter
		const SlaveEmitter * emitter = static_cast<const SlaveEmitter *>(element);

		// Write the header of the SlaveEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getMasterTechniqueName() != BLANK_STRING) {
			serializer->writeLine(token[TOKEN_MASTER_TECHNIQUE], emitter->getMasterTechniqueName(), 12);
		}
		if (emitter->getMasterEmitterName() != BLANK_STRING) {
			serializer->writeLine(token[TOKEN_MASTER_EMITTER], emitter->getMasterEmitterName(), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
