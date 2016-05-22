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

#include "ParticleUniverseEventHandlerTokens.h"

#include "ParticleUniverseEventHandlerFactory.h"
#include "ParticleUniverseObserver.h"
#include "ParticleUniverseSystemManager.h"

namespace ParticleUniverse {

	/**************************************************************************
	 * ParticleEventHandlerTranslator
	 *************************************************************************/
	ParticleEventHandlerTranslator::ParticleEventHandlerTranslator() : mParticleEventHandler(nullptr) {
	}

	void ParticleEventHandlerTranslator::translate(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ObjectAbstractNode * parent = obj->parent ? reinterpret_cast<ObjectAbstractNode *>(obj->parent) : nullptr;

		// The name of the obj is the type of the ParticleEventHandler
		// Remark: This can be solved by using a listener, so that obj->values is filled with type + name. Something for later
		String type;
		if (!obj->name.empty()) {
			type = obj->name;
		} else {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Get the factory
		ParticleEventHandlerFactory * particleEventHandlerFactory = ParticleSystemManager::getSingletonPtr()->getEventHandlerFactory(type);
		if (!particleEventHandlerFactory) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Create the ParticleEventHandler
		mParticleEventHandler = ParticleSystemManager::getSingletonPtr()->createEventHandler(type);
		if (!mParticleEventHandler) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		if (!obj->parent->context.isEmpty()) {
			ParticleObserver * observer = any_cast<ParticleObserver *>(obj->parent->context);
			observer->addEventHandler(mParticleEventHandler);
		} else {
			// It is an alias
			mParticleEventHandler->setAliasName(parent->name);
			ParticleSystemManager::getSingletonPtr()->addAlias(mParticleEventHandler);
		}

		// The first value is the (optional) name
		String name;
		if (!obj->values.empty()) {
			getString(obj->values.front(), &name);
			mParticleEventHandler->setName(name);
		}

		// Set it in the context
		obj->context = Any(mParticleEventHandler);

		// Run through properties
		for (AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i) {
			// No properties of its own
			if ((*i)->type == ANT_PROPERTY) {
				PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>((*i).get());
				if (particleEventHandlerFactory->translateChildProperty(compiler, *i)) {
					// Parsed the property by another translator; do nothing
				} else {
					errorUnexpectedProperty(compiler, prop);
				}
			} else if ((*i)->type == ANT_OBJECT) {
				if (particleEventHandlerFactory->translateChildObject(compiler, *i)) {
					// Parsed the object by another translator; do nothing
				} else {
					processNode(compiler, *i);
				}
			} else {
				errorUnexpectedToken(compiler, *i);
			}
		}
	}

	void ParticleEventHandlerWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Nothing yet
	}

} /* namespace ParticleUniverse */
