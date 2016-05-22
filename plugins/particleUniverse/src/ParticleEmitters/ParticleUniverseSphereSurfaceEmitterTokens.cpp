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

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitterTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEmitters/ParticleUniverseSphereSurfaceEmitter.h"

namespace ParticleUniverse {
	
	bool SphereSurfaceEmitterTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEmitter * em = any_cast<ParticleEmitter *>(prop->parent->context);
		SphereSurfaceEmitter * emitter = static_cast<SphereSurfaceEmitter *>(em);

		if (prop->name == token[TOKEN_RADIUS]) {
			// Property: radius
			if (passValidateProperty(compiler, prop, token[TOKEN_RADIUS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setRadius(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SPHERE_RADIUS]) {
			// Property: sphere_surface_em_radius (deprecated and replaced by 'radius')
			if (passValidateProperty(compiler, prop, token[TOKEN_SPHERE_RADIUS], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					emitter->setRadius(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool SphereSurfaceEmitterTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void SphereSurfaceEmitterWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a SphereSurfaceEmitter
		const SphereSurfaceEmitter * emitter = static_cast<const SphereSurfaceEmitter *>(element);

		// Write the header of the SphereSurfaceEmitter
		serializer->writeLine(token[TOKEN_EMITTER], emitter->getEmitterType(), emitter->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleEmitterWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(emitter->getRadius(), SphereSurfaceEmitter::DEFAULT_RADIUS)) {
			serializer->writeLine(token[TOKEN_RADIUS], StringConverter::toString(emitter->getRadius()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
