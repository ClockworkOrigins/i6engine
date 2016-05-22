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

#ifndef __PU_PARTICLE_AFFECTOR_TOKENS_H__
#define __PU_PARTICLE_AFFECTOR_TOKENS_H__

#include "ParticleUniverseScriptCompiler.h"
#include "ParticleUniverseScriptDeserializer.h"
#include "ParticleUniverseScriptWriter.h"

//#include "ParticleUniverseAffector.h"

namespace ParticleUniverse {

	/** The AffectorTranslator parses 'affector' tokens
	*/
	class _ParticleUniverseExport AffectorTranslator : public ScriptTranslator {
	public:
		AffectorTranslator();
		virtual ~AffectorTranslator() {}
		virtual void translate(ScriptCompiler * compiler, const AbstractNodePtr & node);

	protected:
		ParticleAffector * mAffector;
	};

	/** 
    */
	class _ParticleUniverseExport ParticleAffectorWriter : public ScriptWriter {
	public:
		static const String KEYWORD_AFFECTOR;
		static const String KEYWORD_EXTERN;
		static const String KEYWORD_ENABLED;
		static const String KEYWORD_POSITION;
		static const String KEYWORD_MASS;
		static const String KEYWORD_EXCLUDE_EMITTER;
		static const String KEYWORD_SPECIALISATION;
		static const String KEYWORD_SPEC_DEFAULT;
		static const String KEYWORD_SPEC_TTL_INCREASE;
		static const String KEYWORD_SPEC_TTL_DECREASE;

		ParticleAffectorWriter() {}
		virtual ~ParticleAffectorWriter() {}

		/** @see
			ScriptWriter::write
		*/
		virtual void write(ParticleScriptSerializer * serializer, const IElement * element);
	};

} /* namespace ParticleUniverse */

#endif /* __PU_PARTICLE_AFFECTOR_TOKENS_H__ */
