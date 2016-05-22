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

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandlerTokens.h"

#include "ParticleUniverseScriptSerializer.h"

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

namespace ParticleUniverse {
	
	bool DoEnableComponentEventHandlerTranslator::translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>(node.get());
		ParticleEventHandler * evt = any_cast<ParticleEventHandler *>(prop->parent->context);
		DoEnableComponentEventHandler * handler = static_cast<DoEnableComponentEventHandler *>(evt);

		if (prop->name == token[TOKEN_DOENABLE_COMPONENT]) {
			// Property: enable_component
			if (passValidatePropertyNumberOfValues(compiler, prop, token[TOKEN_DOENABLE_COMPONENT], 3)) {
				String componentType;
				String name;
				bool enabled = true;
				AbstractNodeList::const_iterator i = prop->values.begin();
				if (getString(*i, &componentType)) {
					if (componentType == token[TOKEN_DOENABLE_EMITTER_COMPONENT]) {
						handler->setComponentType(CT_EMITTER);
					} else if (componentType == token[TOKEN_DOENABLE_AFFECTOR_COMPONENT]) {
						handler->setComponentType(CT_AFFECTOR);
					} else if (componentType == token[TOKEN_DOENABLE_OBSERVER_COMPONENT]) {
						handler->setComponentType(CT_OBSERVER);
					} else if (componentType == token[TOKEN_DOENABLE_TECHNIQUE_COMPONENT]) {
						handler->setComponentType(CT_TECHNIQUE);
					}
					++i;
					if (getString(*i, &name)) {
						handler->setComponentName(name);
						++i;
						if (getBoolean(*i, &enabled)) {
							handler->setComponentEnabled(enabled);
							return true;
						}
					}
				}
			}
		}

		return false;
	}
	
	bool DoEnableComponentEventHandlerTranslator::translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		// No objects
		return false;
	}
	
	void DoEnableComponentEventHandlerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Cast the element to a DoEnableComponentEventHandler
		const DoEnableComponentEventHandler * eventHandler = static_cast<const DoEnableComponentEventHandler *>(element);

		// Write the header of the DoEnableComponentEventHandler
		serializer->writeLine(token[TOKEN_HANDLER], eventHandler->getEventHandlerType(), eventHandler->getName(), 12);
		serializer->writeLine("{", 12);

		// Write base attributes
		ParticleEventHandlerWriter::write(serializer, element);

		// Write own attributes
		String componentType = token[TOKEN_DOENABLE_EMITTER_COMPONENT];
		if (eventHandler->getComponentType() == CT_AFFECTOR) {
			componentType = token[TOKEN_DOENABLE_AFFECTOR_COMPONENT];
		} else if (eventHandler->getComponentType() == CT_TECHNIQUE) {
			componentType = token[TOKEN_DOENABLE_TECHNIQUE_COMPONENT];
		} else if (eventHandler->getComponentType() == CT_OBSERVER) {
			componentType = token[TOKEN_DOENABLE_OBSERVER_COMPONENT];
		}
		serializer->writeLine(token[TOKEN_DOENABLE_COMPONENT], componentType, eventHandler->getComponentName(), StringConverter::toString(eventHandler->isComponentEnabled()), 16);

		// Write the close bracket
		serializer->writeLine("}", 12);
	}

} /* namespace ParticleUniverse */
