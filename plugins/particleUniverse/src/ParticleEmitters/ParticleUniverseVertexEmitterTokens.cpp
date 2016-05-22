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

#include "ParticleEmitters/ParticleUniverseVertexEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniverseVertexEmitter.h"

namespace ParticleUniverse {
	
	bool VertexEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		VertexEmitter * emitter = static_cast<VertexEmitter *>(em);

		if (prop->name == token[TOKEN_STEP]) {
			// Property: step
			if (passValidateProperty(compiler, prop, token[TOKEN_STEP], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setStep(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VERTEX_STEP]) {
			// Property: vertex_em_step (deprecated and replaced by 'step')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_STEP], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setStep(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_NUMBER_OF_SEGMENTS]) {
			// Property: number_of_segments
			if (passValidateProperty(compiler, prop, token[TOKEN_NUMBER_OF_SEGMENTS], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setSegments(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VERTEX_SEGMENTS]) {
			// Property: vertex_em_segments (deprecated and replaced by 'number_of_segments')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_SEGMENTS], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setSegments(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ITERATIONS]) {
			// Property: iterations
			if (passValidateProperty(compiler, prop, token[TOKEN_ITERATIONS], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setIterations(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VERTEX_ITERATIONS]) {
			// Property: vertex_em_iterations (deprecated and replaced by 'iterations')
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_ITERATIONS], VAL_UINT)) {
				uint val = 0;
				if (getUInt(prop->values.front(), &val)) {
					emitter->setIterations(ushort(val));
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MESH_NAME]) {
			// Property: mesh_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_NAME], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					emitter->setMeshName(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_VERTEX_MESH_NAME]) {
			// Property: vertex_em_mesh_name (deprecated and replaced by mesh_name)
			if (passValidateProperty(compiler, prop, token[TOKEN_VERTEX_MESH_NAME], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					emitter->setMeshName(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool VertexEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void VertexEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a VertexEmitter
		const VertexEmitter * emitter = static_cast<const VertexEmitter *>(element);

		// Write the header of the VertexEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (emitter->getStep() != VertexEmitter::DEFAULT_STEP) {
			serializer->writeLine(token[TOKEN_STEP], StringConverter::toString(emitter->getStep()), 12);
		}
		if (emitter->getSegments() != VertexEmitter::DEFAULT_SEGMENTS) {
			serializer->writeLine(token[TOKEN_NUMBER_OF_SEGMENTS], StringConverter::toString(emitter->getSegments()), 12);
		}
		if (emitter->getIterations() != VertexEmitter::DEFAULT_ITERATIONS) {
			serializer->writeLine(token[TOKEN_ITERATIONS], StringConverter::toString(emitter->getIterations()), 12);
		}
		if (emitter->getMeshName() != BLANK_STRING) {
			serializer->writeLine(token[TOKEN_MESH_NAME], emitter->getMeshName(), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
