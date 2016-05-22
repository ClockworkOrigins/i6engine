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

#ifndef __PU_GEOMETRY_ROTATOR_TOKENS_H__
#define __PU_GEOMETRY_ROTATOR_TOKENS_H__

#include "ParticleUniverseAffectorTokens.h"

namespace ParticleUniverse {

	/** The GeometryRotatorTranslator parses 'GeometryRotator' tokens
	*/
	class _ParticleUniverseExport GeometryRotatorTranslator : public ScriptTranslator {
	public:
		GeometryRotatorTranslator() {}
		~GeometryRotatorTranslator() {}
		virtual bool translateChildProperty(ScriptCompiler * compiler, const AbstractNodePtr & node);
		virtual bool translateChildObject(ScriptCompiler * compiler, const AbstractNodePtr & node);
	};

	/** 
    */
	class _ParticleUniverseExport GeometryRotatorWriter : public ParticleAffectorWriter {
	public:
		GeometryRotatorWriter() {}
		virtual ~GeometryRotatorWriter() {}

		/** @see
			ScriptWriter::write
		*/
		virtual void write(ParticleScriptSerializer * serializer , const IElement * element);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_GEOMETRY_ROTATOR_TOKENS_H__ */
