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

#include "ParticleEmitters/ParticleUniverseLineEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniverseLineEmitter.h"

namespace ParticleUniverse {
	
	bool LineEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		LineEmitter * emitter = static_cast<LineEmitter *>(em);

		if (prop->name == token[TOKEN_END]) {
			// Property: end
			if (passValidateProperty(compiler, prop, token[TOKEN_END], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					emitter->setEnd(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_LINE_EMIT_END]) {
			// Property: line_em_end (deprecated and replaced by 'end')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_END], VAL_VECTOR3)) {
				Vector3 val;
				if (getVector3(prop->values.begin(), prop->values.end(), &val)) {
					emitter->setEnd(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_INCREMENT]) {
			// Property: max_increment
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_INCREMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMaxIncrement(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_LINE_EMIT_MAX_INCREMENT]) {
			// Property: line_em_max_increment (de[recated and replaced by 'max_increment')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MAX_INCREMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMaxIncrement(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MIN_INCREMENT]) {
			// Property: min_increment
			if (passValidateProperty(compiler, prop, token[TOKEN_MIN_INCREMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMinIncrement(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_LINE_EMIT_MIN_INCREMENT]) {
			// Property: line_em_min_increment (deprecated and replaced by 'min_increment')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MIN_INCREMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMinIncrement(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_DEVIATION]) {
			// Property: max_deviation
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DEVIATION], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMaxDeviation(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_LINE_EMIT_MAX_DEVIATION]) {
			// Property: line_em_max_deviation (deprecated and replaced by 'max_deviation')
			if (passValidateProperty(compiler, prop, token[TOKEN_LINE_EMIT_MAX_DEVIATION], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setMaxDeviation(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool LineEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void LineEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a LineEmitter
		const LineEmitter * emitter = static_cast<const LineEmitter *>(element);

		// Write the header of the LineEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getEnd() != LineEmitter::DEFAULT_END) {
			serializer->writeLine(token[TOKEN_END], StringConverter::toString(emitter->getEnd()), 12);
		}
		if (!almostEquals(emitter->getMinIncrement(), LineEmitter::DEFAULT_MIN_INCREMENT)) {
			serializer->writeLine(token[TOKEN_MIN_INCREMENT], StringConverter::toString(emitter->getMinIncrement()), 12);
		}
		if (!almostEquals(emitter->getMaxIncrement(), LineEmitter::DEFAULT_MAX_INCREMENT)) {
			serializer->writeLine(token[TOKEN_MAX_INCREMENT], StringConverter::toString(emitter->getMaxIncrement()), 12);
		}
		if (!almostEquals(emitter->getMaxDeviation(), LineEmitter::DEFAULT_MAX_DEVIATION)) {
			serializer->writeLine(token[TOKEN_MAX_DEVIATION], StringConverter::toString(emitter->getMaxDeviation()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
