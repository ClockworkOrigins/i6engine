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

#ifndef __PU_EVENT_HANDLER_H__
#define __PU_EVENT_HANDLER_H__

#include "ParticleUniverseIAlias.h"
#include "ParticleUniverseIElement.h"

#include "OGRE/OgreVector3.h"

namespace ParticleUniverse {

	/** A ParticleEventHandlers is used to perform a task in case a certain event happens.
	@remarks
		A ParticleEventHandler is associated with a ParticleObserver; The ParticleObserver signals what event occurs,
		while the ParticleEventHandler performs some action.
	*/
	class _ParticleUniverseExport ParticleEventHandler : public IAlias, public IElement {
	public:
		ParticleEventHandler() : IAlias(), IElement(), mParentObserver(nullptr), mName(BLANK_STRING), _mEventHandlerScale(Vector3::UNIT_SCALE) {
			mAliasType = AT_HANDLER;
		}
		virtual ~ParticleEventHandler() {}

		/** Todo
	    */
		const String & getName() const { return mName; }
		void setName(const String & name) { mName = name; }

		/** Todo
	    */
		ParticleObserver * getParentObserver() const { return mParentObserver; }
		void setParentObserver(ParticleObserver * parentObserver) { mParentObserver = parentObserver; }

		/** Todo
	    */
		const String & getEventHandlerType() const { return mEventHandlerType; }
		void setEventHandlerType(const String & eventHandlerType) { mEventHandlerType = eventHandlerType; }

		/** Notify that the event handler is rescaled.
	    */
		virtual void _notifyRescaled(const Vector3 & scale) { _mEventHandlerScale = scale; }

		/** Todo
	    */
		virtual void _handle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) = 0;

		/** Copy attributes to another event handler.
	    */
		virtual void copyAttributesTo(ParticleEventHandler * eventHandler) {
			copyParentAttributesTo(eventHandler);
		}

		/** Copy parent attributes to another event handler.
	    */
		virtual void copyParentAttributesTo(ParticleEventHandler * eventHandler) {
			eventHandler->setName(mName);
			eventHandler->mParentObserver = mParentObserver;
			eventHandler->_mEventHandlerScale = _mEventHandlerScale;
		}

	protected:
		// Observer to which the eventhandler is associated.
		ParticleObserver * mParentObserver;

		// Type of event handler
		String mEventHandlerType;

		// Name of the eventHandler (optional)
		String mName;

		/** Although the scale is on a Particle System level, the event handler can also be scaled.
		*/
		Vector3 _mEventHandlerScale;
	};

} /* namespace ParticleUniverse */

#endif /* __PU_EVENT_HANDLER_H__ */
