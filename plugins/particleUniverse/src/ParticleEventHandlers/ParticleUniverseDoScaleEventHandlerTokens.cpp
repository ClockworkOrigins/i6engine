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

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandlerTokens.h"

#include "ParticleUniverseAny.h"
#include "ParticleUniverseCommon.h"
#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEventHandlers/ParticleUniverseDoScaleEventHandler.h"

namespace ParticleUniverse {
	
	bool DoScaleEventHandlerTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEventHandler * evt = any_cast<ParticleEventHandler *>(prop->parent->context);
		DoScaleEventHandler * handler = static_cast<DoScaleEventHandler *>(evt);

		if (prop->name == token[TOKEN_DOSCALE_FRACTION]) {
			// Property: scale_fraction
			if (passValidateProperty(compiler, prop, token[TOKEN_DOSCALE_FRACTION], VAL_REAL)) {
				Real val = 0.0;
				if (getReal(prop->values.front(), &val)) {
					handler->setScaleFraction(val);
					return true;
				}
			}
		} else if (prop->name == token[TOKEN_DOSCALE_TYPE]) {
			// Property: scale_type
			if (passValidateProperty(compiler, prop, token[TOKEN_DOSCALE_TYPE], VAL_STRING)) {
				String val;
				if (getString(prop->values.front(), &val)) {
					if (val == token[TOKEN_TIME_TO_LIVE] || val == token[TOKEN_DOSCALE_TIME_TO_LIVE]) {
						handler->setScaleType(DoScaleEventHandler::ST_TIME_TO_LIVE);
						return true;
					} else if (val == token[TOKEN_VELOCITY] || val == token[TOKEN_DOSCALE_VELOCITY]) {
						handler->setScaleType(DoScaleEventHandler::ST_VELOCITY);
						return true;
					}
				}
			}
		}

		return false;
	}
	
	bool DoScaleEventHandlerTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void DoScaleEventHandlerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a DoScaleEventHandler
		const DoScaleEventHandler * eventHandler = static_cast<const DoScaleEventHandler *>(element);

		// Write the header of the DoScaleEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		if (!almostEquals(eventHandler->getScaleFraction(), DoScaleEventHandler::DEFAULT_SCALE_FRACTION)) {
			serializer->writeLine(token[TOKEN_DOSCALE_FRACTION], StringConverter::toString(eventHandler->getScaleFraction()), 16);
		}
		String scaleType = token[TOKEN_DOSCALE_TIME_TO_LIVE];
		if (eventHandler->getScaleType() == DoScaleEventHandler::ST_VELOCITY) {
			scaleType = token[TOKEN_DOSCALE_VELOCITY];
		}
		serializer->writeLine(token[TOKEN_DOSCALE_TYPE], scaleType, 16);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

} /* namespace ParticleUniverse */
