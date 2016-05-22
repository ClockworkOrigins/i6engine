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

#include "ParticleUniverseBehaviourTokens.h"

#include "ParticleUniverseBehaviourFactory.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	/**************************************************************************
	 * BehaviourTranslator
	 *************************************************************************/
	BehaviourTranslator::BehaviourTranslator() : mBehaviour(nullptr) {
	}

	void BehaviourTranslator::translate(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ObjectAbstractNode * parent = obj->parent ? reinterpret_cast<ObjectAbstractNode *>(obj->parent) : nullptr;

		// The name of the obj is the type of the Behaviour
		String type;
		if (!obj->name.empty()) {
			type = obj->name;
		} else {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Get the factory
		ParticleBehaviourFactory * behaviourFactory = ParticleSystemManager::getSingletonPtr()->getBehaviourFactory(type);
		if (!behaviourFactory) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Create the Behaviour
		mBehaviour = ParticleSystemManager::getSingletonPtr()->createBehaviour(type);
		if (!mBehaviour) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		if (!obj->parent->context.isEmpty()) {
			ParticleTechnique * technique = any_cast<ParticleTechnique*>(obj->parent->context);
			technique->_addBehaviourTemplate(mBehaviour);
		} else {
			// It is an alias
			mBehaviour->setAliasName(parent->name);
			ParticleSystemManager::getSingletonPtr()->addAlias(mBehaviour);
		}

		// Set it in the context
		obj->context = Any(mBehaviour);

		// Run through properties
		for (AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i) {
			// No properties of its own
			if ((*i)->type == ANT_PROPERTY) {
				PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>((*i).get());
				if (behaviourFactory->translateChildProperty(compiler, *i)) {
					// Parsed the property by another translator; do nothing
				} else {
					errorUnexpectedProperty(compiler, prop);
				}
			} else if ((*i)->type == ANT_OBJECT) {
				if (behaviourFactory->translateChildObject(compiler, *i)) {
					// Parsed the object by another translator; do nothing
				} else {
					processNode(compiler, *i);
				}
			} else {
				errorUnexpectedToken(compiler, *i);
			}
		}
	}

	void ParticleBehaviourWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Nothing yet
	}

} /* namespace ParticleUniverse */
