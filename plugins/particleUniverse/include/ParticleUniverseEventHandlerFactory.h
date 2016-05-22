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

#ifndef __PU_EVENT_HANDLER_FACTORY_H__
#define __PU_EVENT_HANDLER_FACTORY_H__

#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseEventHandlerTokens.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse {

	/** This is the base factory of all ParticleEventHandler implementations.
    */
	class _ParticleUniverseExport ParticleEventHandlerFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc {
	public:
		ParticleEventHandlerFactory() {}
	    virtual ~ParticleEventHandlerFactory() {}

		/** Returns the type of the factory, which identifies the event handler type this factory creates. */
		virtual String getEventHandlerType() const = 0;

		/** Creates a new event handler instance.
		@remarks
	    */
		virtual ParticleEventHandler * createEventHandler() = 0;

		/** Delete an event handler
	    */
		void destroyEventHandler(ParticleEventHandler * eventHandler) {
			PU_DELETE_T(eventHandler, ParticleEventHandler, MEMCATEGORY_SCENE_OBJECTS);
		}

	protected:
		/**
		*/
		template<class T>
		ParticleEventHandler * _createEventHandler() {
			ParticleEventHandler * particleEventHandler = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
			particleEventHandler->setEventHandlerType(getEventHandlerType());
			return particleEventHandler;
		}
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EVENT_HANDLER_FACTORY_H__ */
