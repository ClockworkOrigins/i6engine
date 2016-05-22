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

#ifndef __PU_EXTERN_FACTORY_H__
#define __PU_EXTERN_FACTORY_H__

#include "ParticleUniverseExtern.h"
#include "ParticleUniverseExternTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all Extern implementations.
    */
	class _ParticleUniverseExport ExternFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
		ExternFactory() {}
		virtual ~ExternFactory() {}

		/** Returns the type of the factory, which identifies the Extern type this factory creates. */
		virtual String getExternType() const = 0;

		/** Creates a new Extern instance.
		@remarks
	    */
		virtual Extern * createExtern() = 0;

		/** Delete an Extern.
	    */
		void destroyExtern(Extern * externObject) {
			if (externObject) {
				PU_DELETE externObject;
			}
		}

	protected:
		/**
		*/
		template<class T>
		Extern * _createExtern() {
			Extern * externObject = PU_NEW T();
			externObject->setExternType(getExternType());
			return externObject;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EXTERN_FACTORY_H__ */
