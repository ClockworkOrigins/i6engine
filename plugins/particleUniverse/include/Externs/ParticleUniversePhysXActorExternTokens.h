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

#ifndef __PU_PHYSX_EXTERN_TOKENS_H__
#define __PU_PHYSX_EXTERN_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#ifdef PU_PHYSICS_PHYSX
	#include "ParticleUniverseExternTokens.h"

	namespace ParticleUniverse
	{
		/** The PhysXActorExternTranslator parses 'PhysXActorExtern' tokens
		*/
		class _ParticleUniverseExport PhysXActorExternTranslator : public ScriptTranslator
		{
			public:
				PhysXActorExternTranslator(void){};
				~PhysXActorExternTranslator(void){};
				virtual bool translateChildProperty(ScriptCompiler* compiler, const AbstractNodePtr &node);
				virtual bool translateChildObject(ScriptCompiler* compiler, const AbstractNodePtr &node);
		};
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		/** The PhysXShapeTranslator also parses 'PhysXActorExtern' tokens
		*/
		class _ParticleUniverseExport PhysXShapeTranslator : public ScriptTranslator
		{
			public:
				PhysXShapeTranslator(void){};
				~PhysXShapeTranslator(void){};
				virtual void translate(ScriptCompiler* compiler, const AbstractNodePtr &node);
		};
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------

		/** 
		*/
		class _ParticleUniverseExport PhysXActorExternWriter : public ExternWriter
		{
			public:

				PhysXActorExternWriter(void) {};
				virtual ~PhysXActorExternWriter(void) {};

				/** @see
					ScriptWriter::write
				*/
				virtual void write(ParticleScriptSerializer* serializer , const IElement* element);
		};

	}
#endif //PU_PHYSICS_PHYSX
#endif
