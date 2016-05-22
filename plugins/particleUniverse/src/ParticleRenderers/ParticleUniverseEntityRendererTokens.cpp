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

#include "ParticleRenderers/ParticleUniverseEntityRendererTokens.h"

#include "ParticleUniverseAny.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleRenderers/ParticleUniverseEntityRenderer.h"

namespace ParticleUniverse {
	
	bool EntityRendererTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleRenderer * ren = any_cast<ParticleRenderer *>(prop->parent->context);
		EntityRenderer * renderer = static_cast<EntityRenderer *>(ren);

		if (prop->name == token[TOKEN_MESH_NAME]) {
			// Property: mesh_name
			if (passValidateProperty(compiler, prop, token[TOKEN_MESH_NAME], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					renderer->setMeshName(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ENT_MESH_NAME]) {
			// Property: entity_renderer_mesh_name (deprecated and replaced by mesh_name)
			if (passValidateProperty(compiler, prop, token[TOKEN_ENT_MESH_NAME], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					renderer->setMeshName(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ENT_ORIENTATION_TYPE]) {
			// Property: entity_orientation_type
			if (passValidateProperty(compiler, prop, token[TOKEN_ENT_ORIENTATION_TYPE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_ENT_ORIENTED_SELF]) {
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SELF);
						return true;
					} else if (val == token[TOKEN_ENT_ORIENTED_SELF_MIRRORED]) {
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SELF_MIRRORED);
						return true;
					} else if (val == token[TOKEN_ENT_ORIENTED_SHAPE]) {
						renderer->setEntityOrientationType(EntityRenderer::ENT_ORIENTED_SHAPE);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool EntityRendererTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void EntityRendererWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a EntityRenderer
		const EntityRenderer * renderer = static_cast<const EntityRenderer *>(element);

		// Write the header of the EntityRenderer
		serializer->writeLine(token[TOKEN_RENDERER], renderer->getRendererType(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleRendererWriter::write(serializer, element);

		// Write own attributes
		if (renderer->getMeshName() != BLANK_STRING) {
			serializer->writeLine(token[TOKEN_MESH_NAME], renderer->getMeshName(), 12);
		}
		if (renderer->getEntityOrientationType() != EntityRenderer::DEFAULT_ORIENTATION_TYPE) {
			String orientationType = token[TOKEN_ENT_ORIENTED_SELF];
			if (renderer->getEntityOrientationType() == EntityRenderer::ENT_ORIENTED_SHAPE) {
				orientationType = token[TOKEN_ENT_ORIENTED_SELF];
			} else if (renderer->getEntityOrientationType() == EntityRenderer::ENT_ORIENTED_SELF_MIRRORED) {
				orientationType = token[TOKEN_ENT_ORIENTED_SELF_MIRRORED];
			}
			serializer->writeLine(token[TOKEN_ENT_ORIENTATION_TYPE], orientationType, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
