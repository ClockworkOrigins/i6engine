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

#include "ParticleAffectors/ParticleUniverseInterParticleColliderTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseInterParticleCollider.h"

namespace ParticleUniverse {
	
	bool InterParticleColliderTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		InterParticleCollider * affector = static_cast<InterParticleCollider *>(af);

		if (prop->name == token[TOKEN_ADJUSTMENT]) {
			// Property: adjustment
			if (passValidateProperty(compiler, prop, token[TOKEN_ADJUSTMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setAdjustment(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_IPC_ADJUSTMENT]) {
			// Property: ip_adjustment (deprecated and replaced by 'adjustment')
			if (passValidateProperty(compiler, prop, token[TOKEN_IPC_ADJUSTMENT], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setAdjustment(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_COLLISION_RESPONSE]) {
			// Property: collision_response
			if (passValidateProperty(compiler, prop, token[TOKEN_COLLISION_RESPONSE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_IPC_AVERAGE_VELOCITY]) {
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_AVERAGE_VELOCITY);
						return true;
					} else if (val == token[TOKEN_IPC_ANGLE_BASED_VELOCITY]) {
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
						return true;
					}
				}
			}
		} else if (prop->name == token[TOKEN_IPC_COLLISION_RESPONSE]) {
			// Property: ip_collision_response (deprecated and replaced by 'collision_response')
			if (passValidateProperty(compiler, prop, token[TOKEN_IPC_COLLISION_RESPONSE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_IPC_AVERAGE_VELOCITY]) {
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_AVERAGE_VELOCITY);
						return true;
					} else if (val == token[TOKEN_IPC_ANGLE_BASED_VELOCITY]) {
						affector->setInterParticleCollisionResponse(InterParticleCollider::IPCR_ANGLE_BASED_VELOCITY);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool InterParticleColliderTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void InterParticleColliderWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a InterParticleCollider
		const InterParticleCollider * affector = static_cast<const InterParticleCollider *>(element);

		// Write the header of the InterParticleCollider
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getAdjustment(), InterParticleCollider::DEFAULT_ADJUSTMENT)) {
			serializer->writeLine(token[TOKEN_ADJUSTMENT], StringConverter::toString(affector->getAdjustment()), 12);
		}

		if (affector->getInterParticleCollisionResponse() != InterParticleCollider::DEFAULT_COLLISION_RESPONSE) {
			String collisionResponse = token[TOKEN_IPC_ANGLE_BASED_VELOCITY];
			if (affector->getInterParticleCollisionResponse() == InterParticleCollider::IPCR_AVERAGE_VELOCITY) {
				collisionResponse = token[TOKEN_IPC_AVERAGE_VELOCITY];
			}
			serializer->writeLine(token[TOKEN_COLLISION_RESPONSE], collisionResponse, 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
