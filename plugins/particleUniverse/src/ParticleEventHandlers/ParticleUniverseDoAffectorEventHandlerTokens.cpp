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

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandlerTokens.h"

#include "ParticleUniverseAny.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

namespace ParticleUniverse {
	
	bool DoAffectorEventHandlerTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEventHandler * evt = any_cast<ParticleEventHandler *>(prop->parent->context);
		DoAffectorEventHandler * handler = static_cast<DoAffectorEventHandler *>(evt);

		if (prop->name == token[TOKEN_FORCE_AFFECTOR]) {
			// Property: force_affector
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFFECTOR], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					handler->setAffectorName(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_FORCE_AFFECTOR_PRE_POST]) {
			// Property: pre_post
			if (passValidateProperty(compiler, prop, token[TOKEN_FORCE_AFFECTOR_PRE_POST], VAL_BOOL)) {
				bool val;
				if (getBoolean(prop->values.front(), &val)) {
					handler->setPrePost(val);
					return true;
				}
			}
		}

		return false;
	}
	
	bool DoAffectorEventHandlerTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void DoAffectorEventHandlerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a DoAffectorEventHandler
		const DoAffectorEventHandler * eventHandler = static_cast<const DoAffectorEventHandler *>(element);

		// Write the header of the DoAffectorEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		if (eventHandler->getAffectorName() != BLANK_STRING) {
			serializer->writeLine(token[TOKEN_FORCE_AFFECTOR], eventHandler->getAffectorName(), 16);
		}
		if (eventHandler->getPrePost() != DoAffectorEventHandler::DEFAULT_PRE_POST) {
			serializer->writeLine(token[TOKEN_FORCE_AFFECTOR_PRE_POST], StringConverter::toString(eventHandler->getPrePost()), 16);
		}

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

} /* namespace ParticleUniverse */
