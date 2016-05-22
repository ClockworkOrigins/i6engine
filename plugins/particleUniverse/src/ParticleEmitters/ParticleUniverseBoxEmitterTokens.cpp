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

#include "ParticleEmitters/ParticleUniverseBoxEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniverseBoxEmitter.h"

namespace ParticleUniverse {
	
	bool BoxEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		BoxEmitter * emitter = static_cast<BoxEmitter *>(em);

		if (prop->name == token[TOKEN_BOX_WIDTH]) {
			// Property: box_width
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_WIDTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setWidth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_EMITTER_BOX_WIDTH]) {
			// Property: box_em_width (deprecated and replaced by 'box_width')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_WIDTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setWidth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOX_HEIGHT]) {
			// Property: box_height
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_HEIGHT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setHeight(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_EMITTER_BOX_HEIGHT]) {
			// Property: box_em_height (deprecated and replaced by 'box_height')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_HEIGHT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setHeight(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_BOX_DEPTH]) {
			// Property: box_depth
			if (passValidateProperty(compiler, prop, token[TOKEN_BOX_DEPTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setDepth(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_EMITTER_BOX_DEPTH]) {
			// Property: box_em_depth (deprecated and replaced by 'box_depth')
			if (passValidateProperty(compiler, prop, token[TOKEN_EMITTER_BOX_DEPTH], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setDepth(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool BoxEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void BoxEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a BoxEmitter
		const BoxEmitter * emitter = static_cast<const BoxEmitter *>(element);

		// Write the header of the BoxEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(emitter->getWidth(), BoxEmitter::DEFAULT_WIDTH)) {
			serializer->writeLine(token[TOKEN_BOX_WIDTH], StringConverter::toString(emitter->getWidth()), 12);
		}
		if (!almostEquals(emitter->getHeight(), BoxEmitter::DEFAULT_HEIGHT)) {
			serializer->writeLine(token[TOKEN_BOX_HEIGHT], StringConverter::toString(emitter->getHeight()), 12);
		}
		if (!almostEquals(emitter->getDepth(), BoxEmitter::DEFAULT_DEPTH)) {
			serializer->writeLine(token[TOKEN_BOX_DEPTH], StringConverter::toString(emitter->getDepth()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
