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

#include "ParticleUniverseAliasTokens.h"

namespace ParticleUniverse {

	/**************************************************************************
	 * AliasTranslator
	 *************************************************************************/
	AliasTranslator::AliasTranslator() {
	}

	void AliasTranslator::translate(ScriptCompiler * compiler, const AbstractNodePtr & node) {
		ObjectAbstractNode * obj = reinterpret_cast<ObjectAbstractNode *>(node.get());
		if (obj->name.empty()) {
			compiler->addError(ScriptCompiler::CE_OBJECTNAMEEXPECTED, obj->file, int(obj->line));
		}

		for (AbstractNodeList::iterator i = obj->children.begin(); i != obj->children.end(); ++i) {
			if ((*i)->type == ANT_OBJECT) {
				processNode(compiler, *i);
			}
		}
	}

} /* namespace ParticleUniverse */
