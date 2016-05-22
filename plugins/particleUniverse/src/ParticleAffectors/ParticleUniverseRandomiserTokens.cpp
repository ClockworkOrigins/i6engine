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

#include "ParticleAffectors/ParticleUniverseRandomiserTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseRandomiser.h"

namespace ParticleUniverse {
	
	bool RandomiserTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		Randomiser * affector = static_cast<Randomiser *>(af);

		if (prop->name == token[TOKEN_MAX_DEVIATION_X]) {
			// Property: max_deviation_x
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DEVIATION_X], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationX(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_RND_MAX_DEVIATION_X]) {
			// Property: rand_aff_max_deviation_x (depreacted and replaced by 'max_deviation_x')
			if (passValidateProperty(compiler, prop, token[TOKEN_RND_MAX_DEVIATION_X], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationX(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_DEVIATION_Y]) {
			// Property: max_deviation_y
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DEVIATION_Y], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationY(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_RND_MAX_DEVIATION_Y]) {
			// Property: rand_aff_max_deviation_y (deprecated and replaced by 'max_deviation_y')
			if (passValidateProperty(compiler, prop, token[TOKEN_RND_MAX_DEVIATION_Y], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationY(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_DEVIATION_Z]) {
			// Property: max_deviation_z
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_DEVIATION_Z], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationZ(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_RND_MAX_DEVIATION_Z]) {
			// Property: rand_aff_max_deviation_z (deprecated and replaced by 'max_deviation_z')
			if (passValidateProperty(compiler, prop, token[TOKEN_RND_MAX_DEVIATION_Z], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setMaxDeviationZ(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_TIME_STEP]) {
			// Property: time_step
			if (passValidateProperty(compiler, prop, token[TOKEN_TIME_STEP], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setTimeStep(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_RND_TIME_STEP]) {
			// Property: rand_aff_time_step (deprecated and replaced by 'time_step')
			if (passValidateProperty(compiler, prop, token[TOKEN_RND_TIME_STEP], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setTimeStep(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_USE_DIRECTION]) {
			// Property: use_direction
			if (passValidateProperty(compiler, prop, token[TOKEN_USE_DIRECTION], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setRandomDirection(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_RND_DIRECTION]) {
			// Property: rand_aff_direction (deprecated and replaced by 'use_direction')
			if (passValidateProperty(compiler, prop, token[TOKEN_RND_DIRECTION], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setRandomDirection(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool RandomiserTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void RandomiserWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a Randomiser
		const Randomiser * affector = static_cast<const Randomiser *>(element);

		// Write the header of the Randomiser
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getMaxDeviationX(), Randomiser::DEFAULT_MAX_DEVIATION.x)) {
			serializer->writeLine(token[TOKEN_MAX_DEVIATION_X], StringConverter::toString(affector->getMaxDeviationX()), 12);
		}
		if (!almostEquals(affector->getMaxDeviationY(), Randomiser::DEFAULT_MAX_DEVIATION.y)) {
			serializer->writeLine(token[TOKEN_MAX_DEVIATION_Y], StringConverter::toString(affector->getMaxDeviationY()), 12);
		}
		if (!almostEquals(affector->getMaxDeviationZ(), Randomiser::DEFAULT_MAX_DEVIATION.z)) {
			serializer->writeLine(token[TOKEN_MAX_DEVIATION_Z], StringConverter::toString(affector->getMaxDeviationZ()), 12);
		}
		if (!almostEquals(affector->getTimeStep(), Randomiser::DEFAULT_TIME_STEP)) {
			serializer->writeLine(token[TOKEN_TIME_STEP], StringConverter::toString(affector->getTimeStep()), 12);
		}
		if (affector->isRandomDirection() != Randomiser::DEFAULT_RANDOM_DIRECTION) {
			serializer->writeLine(token[TOKEN_USE_DIRECTION], StringConverter::toString(affector->isRandomDirection()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
