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

#include "ParticleUniverseExternTokens.h"

#include "ParticleUniverseExternFactory.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

namespace ParticleUniverse {

	/**************************************************************************
	 * ExternTranslator
	 *************************************************************************/
	ExternTranslator::ExternTranslator() : mExtern(nullptr) {
	}
	
	void ExternTranslator::translate(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
		ObjectAbstractNode * parent = obj->parent ? reinterpret_cast<ObjectAbstractNode *>(obj->parent) : nullptr;

		// The name of the obj is the type of the Extern
		// Remark: This can be solved by using a listener, so that obj->values is filled with type + name. Something for later
		String type;
		if (!obj->name.empty()) {
			type = obj->name;
		} else {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Get the factory
		ExternFactory * externFactory = ParticleSystemManager::getSingletonPtr()->getExternFactory(type);
		if (!externFactory) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		// Create the Extern
		mExtern = ParticleSystemManager::getSingletonPtr()->createExtern(type);
		if (!mExtern) {
			compiler->addError(ScriptCompiler::CE_INVALIDPARAMETERS, obj->file, int(obj->line));
			return;
		}

		if (!obj->parent->context.isEmpty()) {
			ParticleTechnique * technique = any_cast<ParticleTechnique *>(obj->parent->context);
			technique->addExtern(mExtern);
		} else {
			// It is an alias
			mExtern->setAliasName(parent->name);
			ParticleSystemManager::getSingletonPtr()->addAlias(mExtern);
		}

		// The first value is the (optional) name
		String name;
		if (!obj->values.empty()) {
			getString(obj->values.front(), &name);
			mExtern->setName(name);
		}

		// Set it in the context
		obj->context = Any(mExtern);

		// Run through properties
		for (AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i) {
			// No properties of its own
			if ((*i)->type == ANT_PROPERTY) {
				PropertyAbstractNode * prop = reinterpret_cast<PropertyAbstractNode *>((*i).get());
				if (externFactory->translateChildProperty(compiler, *i)) {
					// Parsed the property by another translator; do nothing
				} else {
					errorUnexpectedProperty(compiler, prop);
				}
			} else if ((*i)->type == ANT_OBJECT) {
				if (externFactory->translateChildObject(compiler, *i)) {
					// Parsed the object by another translator; do nothing
				} else {
					processNode(compiler, *i);
				}
			} else {
				errorUnexpectedToken(compiler, *i);
			}
		}
	}

	void ExternWriter::write(ParticleScriptSerializer * serializer, const IElement * element) {
		// Nothing yet
	}

} /* namespace ParticleUniverse */
