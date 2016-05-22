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

#ifndef __PU_I_ALIAS_H__
#define __PU_I_ALIAS_H__

#include "ParticleUniversePrerequisites.h"

namespace ParticleUniverse {

	/** Abstract (pure virtual) alias class
	@remarks
		The IAlias class acts as an interface and is used to represent all other classes that are a child of the
		IAlias class. This typically concerns entities that are used in the Particle Universe scripts and for
		which it is possible to define an alias (i.e. ParticleTechnique, ParticleEmitter and ParticleAffector).
	*/
	class _ParticleUniverseExport IAlias {
	public:
		enum AliasType {
			AT_UNDEFINED,
			AT_TECHNIQUE,
			AT_RENDERER,
			AT_EMITTER,
			AT_AFFECTOR,
			AT_OBSERVER,
			AT_EXTERN,
			AT_HANDLER,
			AT_BEHAVIOUR
		};

		IAlias() : mAliasName(BLANK_STRING), mAliasType(AT_UNDEFINED) {}
		virtual ~IAlias() {}

		/** 
		*/
		const String & getAliasName() const { return mAliasName; }
		void setAliasName(String aliasName) { mAliasName = aliasName; }

		/** 
		*/
		AliasType getAliasType() const { return mAliasType; }
		void setAliasType(AliasType aliasType) { mAliasType = aliasType; }

	protected:
		String mAliasName;
		AliasType mAliasType;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_I_ALIAS_H__ */
