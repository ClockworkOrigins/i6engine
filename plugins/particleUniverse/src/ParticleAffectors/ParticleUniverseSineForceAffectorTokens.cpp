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

#include "ParticleAffectors/ParticleUniverseSineForceAffectorTokens.h"

#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseSineForceAffector.h"

namespace ParticleUniverse {
	
	bool SineForceAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		SineForceAffector * affector = static_cast<SineForceAffector *>(af);

		if (prop->name == token[TOKEN_MIN_FREQUENCY]) {
			// Property: min_frequency
			if (passValidateProperty(compiler, prop, token[TOKEN_MIN_FREQUENCY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFrequencyMin(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SINE_MIN_FREQUENCY]) {
			// Property: sinef_aff_frequency_min (deprecated and replaced by 'min_frequency')
			if (passValidateProperty(compiler, prop, token[TOKEN_SINE_MIN_FREQUENCY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFrequencyMin(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_MAX_FREQUENCY]) {
			// Property: max_frequency
			if (passValidateProperty(compiler, prop, token[TOKEN_MAX_FREQUENCY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFrequencyMax(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_SINE_MAX_FREQUENCY]) {
			// Property: sinef_aff_frequency_max (deprecated and replaced by 'max_frequency')
			if (passValidateProperty(compiler, prop, token[TOKEN_SINE_MAX_FREQUENCY], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					affector->setFrequencyMax(val);
					return true;
				}
			}
		} else {
			// Parse the BaseForceAffector
			BaseForceAffectorTranslator BaseForceAffectorTranslator;
			return BaseForceAffectorTranslator.translateChildProperty(compiler, node); // Must be the last
		}

		return false;
	}
	
	bool SineForceAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void SineForceAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a SineForceAffector
		const SineForceAffector * affector = static_cast<const SineForceAffector *>(element);

		// Write the header of the SineForceAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		BaseForceAffectorWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(affector->getFrequencyMin(), SineForceAffector::DEFAULT_FREQ_MIN)) {
			serializer->writeLine(token[TOKEN_MIN_FREQUENCY], StringConverter::toString(affector->getFrequencyMin()), 12);
		}
		if (!almostEquals(affector->getFrequencyMax(), SineForceAffector::DEFAULT_FREQ_MAX)) {
			serializer->writeLine(token[TOKEN_MAX_FREQUENCY], StringConverter::toString(affector->getFrequencyMax()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
