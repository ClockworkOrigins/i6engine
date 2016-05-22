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

#include "ParticleAffectors/ParticleUniverseAlignAffectorTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleAffectors/ParticleUniverseAlignAffector.h"

namespace ParticleUniverse {
	
	bool AlignAffectorTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleAffector * af = any_cast<ParticleAffector *>(prop->parent->context);
		AlignAffector * affector = static_cast<AlignAffector *>(af);

		if (prop->name == token[TOKEN_RESIZE]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_RESIZE], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setResize(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_ALIGN_RESIZE]) {
			if (passValidateProperty(compiler, prop, token[TOKEN_ALIGN_RESIZE], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					affector->setResize(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool AlignAffectorTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void AlignAffectorWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a AlignAffector
		const AlignAffector * affector = static_cast<const AlignAffector *>(element);

		// Write the header of the AlignAffector
		serializer->writeLine(token[TOKEN_AFFECTOR], affector->getAffectorType(), affector->getName(), 8);
		serializer->writeLine("{", 8);

		// Write base attributes
		ParticleAffectorWriter::write(serializer, element);

		// Write own attributes
		if (affector->isResize() != AlignAffector::DEFAULT_RESIZE) {
			serializer->writeLine(token[TOKEN_RESIZE], StringConverter::toString(affector->isResize()), 12);
		}

		// Write the close bracket
		serializer->writeLine("}", 8);
	}

} /* namespace ParticleUniverse */
