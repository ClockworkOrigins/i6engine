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

#include "ParticleUniverseSystemManagerTokens.h"

namespace ParticleUniverse {

	BuiltinScriptTranslatorManager::BuiltinScriptTranslatorManager() {
	}
	
	size_t BuiltinScriptTranslatorManager::getNumTranslators() const {
		return 9;
	}
	
	ScriptTranslator * BuiltinScriptTranslatorManager::getTranslator(const AbstractNodePtr & node) {
		ScriptTranslator * translator = nullptr;

		if (node->type == ANT_OBJECT) {
			ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
			ObjectAbstractNode * parent = obj->parent ? reinterpret_cast<ObjectAbstractNode *>(obj->parent) : nullptr;
			if (obj->cls == token[TOKEN_SYSTEM]) {
				// Parse Particle System
				translator = &mSystemTranslator;
			} else if (obj->cls == token[TOKEN_ALIAS]) {
				// Parse the Alias
				translator = &mAliasTranslator;
			} else if (obj->cls == token[TOKEN_TECHNIQUE] && parent && (parent->cls == token[TOKEN_SYSTEM] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Technique
				translator = &mTechniqueTranslator;
			} else if (obj->cls == token[TOKEN_RENDERER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Renderer
				translator = &mRendererTranslator;
			} else if (obj->cls == token[TOKEN_EMITTER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Emitter
				translator = &mEmitterTranslator;
			} else if (obj->cls == token[TOKEN_AFFECTOR] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Affector
				translator = &mAffectorTranslator;
			} else if (obj->cls == token[TOKEN_BEHAVIOUR] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Behaviour
				translator = &mBehaviourTranslator;
			} else if (obj->cls == token[TOKEN_OBSERVER] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Observer
				translator = &mObserverTranslator;
			} else if (obj->cls == token[TOKEN_HANDLER] && parent && (parent->cls == token[TOKEN_OBSERVER] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Particle Event Handler
				translator = &mParticleEventHandlerTranslator;
			} else if (obj->cls == token[TOKEN_EXTERN] && parent && (parent->cls == token[TOKEN_TECHNIQUE] || parent->cls == token[TOKEN_ALIAS])) {
				// Parse Extern
				translator = &mExternTranslator;
			}
		}

		return translator;
	}

} /* namespace ParticleUniverse */
