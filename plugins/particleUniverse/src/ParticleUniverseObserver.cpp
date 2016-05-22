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

#include "ParticleUniverseObserver.h"

#include "ParticleUniverseEventHandler.h"
#include "ParticleUniverseSystemManager.h"

namespace ParticleUniverse {

	// Constants
	const bool ParticleObserver::DEFAULT_ENABLED = true;
	const Particle::ParticleType ParticleObserver::DEFAULT_PARTICLE_TYPE = Particle::PT_VISUAL;
	const Real ParticleObserver::DEFAULT_INTERVAL = 0.05;
	const bool ParticleObserver::DEFAULT_UNTIL_EVENT = false;

	
	ParticleObserver::ParticleObserver() : IAlias(), IElement(), mParentTechnique(nullptr), mName(BLANK_STRING), mEnabled(DEFAULT_ENABLED), mOriginalEnabled(DEFAULT_ENABLED), mOriginalEnabledSet(false), mObserve(true), mObserveUntilEvent(DEFAULT_UNTIL_EVENT), mEventHandlersExecuted(false), _mObserverScale(Vector3::UNIT_SCALE), mParticleTypeToObserve(DEFAULT_PARTICLE_TYPE), mParticleTypeToObserveSet(false), mObserverInterval(DEFAULT_INTERVAL), mObserverIntervalRemainder(0.0), mObserverIntervalSet(false) {
		mAliasType = AT_OBSERVER;
	}
	
	ParticleObserver::~ParticleObserver() {
		destroyAllEventHandlers();
	}
	
	void ParticleObserver::_notifyStart() {
		mEventHandlersExecuted = false;
		mObserve = true;
		setEnabled(mOriginalEnabled);
	}
	
	bool ParticleObserver::isEnabled() const {
		return mEnabled;
	}
	
	bool ParticleObserver::_getOriginalEnabled() const {
		return mOriginalEnabled;
	}
	
	void ParticleObserver::setEnabled(bool enabled) {
		mEnabled = enabled;
		if (!mOriginalEnabledSet) {
			// Only one time is permitted
			mOriginalEnabled = enabled;
			mOriginalEnabledSet = true;
		}
	}
	
	void ParticleObserver::_resetEnabled() {
		mOriginalEnabledSet = false;
	}
	
	Real ParticleObserver::getObserverInterval() const {
		return mObserverInterval;
	}
	
	void ParticleObserver::setObserverInterval(Real observerInterval) {
		mObserverInterval = observerInterval;
		mObserverIntervalSet = true;
	}
	
	bool ParticleObserver::getObserveUntilEvent() const {
		return mObserveUntilEvent;
	}
	
	void ParticleObserver::setObserveUntilEvent(bool observeUntilEvent) {
		mObserveUntilEvent = observeUntilEvent;
	}
	
	void ParticleObserver::_notifyRescaled(const Vector3 & scale) {
		_mObserverScale = scale;

		if (mEventHandlers.empty()) {
			return;
		}

		for (ParticleEventHandlerConstIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			(*it)->_notifyRescaled(scale);
		}
	}
	
	ParticleEventHandler * ParticleObserver::createEventHandler(const String & eventHandlerType) {
		assert(eventHandlerType != BLANK_STRING && "eventHandlerType is empty!");
		ParticleEventHandler * eventHandler = ParticleSystemManager::getSingletonPtr()->createEventHandler(eventHandlerType);
		addEventHandler(eventHandler);
		return eventHandler;
	}
	
	void ParticleObserver::addEventHandler(ParticleEventHandler * eventHandler) {
		assert(eventHandler && "EventHandler is null!");
		mEventHandlers.push_back(eventHandler);
		eventHandler->setParentObserver(this);
		eventHandler->_notifyRescaled(_mObserverScale);
	}
	
	void ParticleObserver::removeEventHandler(ParticleEventHandler * eventHandler) {
		assert(eventHandler && "EventHandler is null!");
		for (ParticleEventHandlerIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			if (*it == eventHandler) {
				// Remove it
				mEventHandlers.erase(it);
				break;
			}
		}

		eventHandler->setParentObserver(nullptr);
	}
	
	ParticleEventHandler * ParticleObserver::getEventHandler(size_t index) const {
		assert(index < mEventHandlers.size() && "EventHandler index out of bounds!");
		return mEventHandlers[index];
	}
	
	ParticleEventHandler * ParticleObserver::getEventHandler(const String & eventHandlerName) const {
		if (eventHandlerName == BLANK_STRING) {
			return nullptr;
		}

		for (ParticleEventHandlerConstIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			if ((*it)->getName() == eventHandlerName) {
				return *it;
			}
		}
		
		return nullptr;
	}
	
	size_t ParticleObserver::getNumEventHandlers() const {
		return mEventHandlers.size();
	}
	
	void ParticleObserver::destroyEventHandler(ParticleEventHandler * eventHandler) {
		assert(eventHandler && "EventHandler is null!");
		for (ParticleEventHandlerIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			if (*it == eventHandler) {
				// Detroy it
				ParticleSystemManager::getSingletonPtr()->destroyEventHandler(*it);
				mEventHandlers.erase(it);
				break;
			}
		}
	}
	
	void ParticleObserver::destroyEventHandler(size_t index) {
		destroyEventHandler(getEventHandler(index));
	}
	
	void ParticleObserver::destroyAllEventHandlers() {
		for (ParticleEventHandlerIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			ParticleSystemManager::getSingletonPtr()->destroyEventHandler(*it);
		}
		mEventHandlers.clear();
	}
	
	void ParticleObserver::copyAttributesTo(ParticleObserver * observer) {
		copyParentAttributesTo(observer);
	}
	
	void ParticleObserver::copyParentAttributesTo(ParticleObserver * observer) {
		// Copy attributes
		observer->setName(mName);
		observer->mParticleTypeToObserve = mParticleTypeToObserve;
		observer->mParticleTypeToObserveSet = mParticleTypeToObserveSet;
		observer->mParentTechnique = mParentTechnique;
		observer->_mObserverScale = _mObserverScale;
		observer->mObserverInterval = mObserverInterval;
		observer->mObserverIntervalRemainder = mObserverIntervalRemainder;
		observer->mObserverIntervalSet = mObserverIntervalSet;
		observer->mObserveUntilEvent = mObserveUntilEvent;
		observer->mEventHandlersExecuted = mEventHandlersExecuted;
        observer->mEnabled = mEnabled;
		observer->mOriginalEnabled = mOriginalEnabled;
		observer->mOriginalEnabledSet = mOriginalEnabledSet;

		// Copy event handlers
		ParticleEventHandler * eventHandler = nullptr;
		ParticleEventHandler * clonedEventHandler = nullptr;
		for (size_t i = 0; i < getNumEventHandlers(); ++i) {
			eventHandler = getEventHandler(i);
			clonedEventHandler = ParticleSystemManager::getSingletonPtr()->cloneEventHandler(eventHandler);
			observer->addEventHandler(clonedEventHandler);
		}
	}
	
	void ParticleObserver::setParticleTypeToObserve(const Particle::ParticleType particleTypeToObserve) {
		mParticleTypeToObserve = particleTypeToObserve;
		mParticleTypeToObserveSet = true;
	}
	
	void ParticleObserver::_preProcessParticles(ParticleTechnique * technique, Real timeElapsed) {
		if (!mEnabled) {
			return;
		}

		if (mObserverIntervalSet) {
			mObserverIntervalRemainder -= timeElapsed;
			if (mObserverIntervalRemainder < 0) {
				mObserverIntervalRemainder += mObserverInterval;
				mObserve = true;
			} else {
				mObserve = false;
			}
		}
	}
	
	void ParticleObserver::_processParticle(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed, bool firstParticle) {
		if (!mEnabled) {
			return;
		}

		// Call the _firstParticle() function if the first particle in the update loop is encountered.
		if (firstParticle) {
			_firstParticle(particleTechnique, particle, timeElapsed);
		}

		if (mParticleTypeToObserveSet && particle->particleType != mParticleTypeToObserve) {
			return;
		}

		// Observe
		_handleObserve(particleTechnique, particle, timeElapsed);
	}
	
	void ParticleObserver::_handleObserve(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (mEnabled && mObserve) {
			if (mObserveUntilEvent && mEventHandlersExecuted) {
				// Don't continue if mObserveUntilEvent is set and the event handlers are already called once.
				return;
			}
			if (_observe(particleTechnique, particle, timeElapsed)) {
				// Handle the event
				_handleEvent (particleTechnique, particle, timeElapsed);
			}
		}
	}
	
	void ParticleObserver::_handleEvent(ParticleTechnique * particleTechnique, Particle * particle, Real timeElapsed) {
		if (mEventHandlers.empty()) {
			return;
		}

		for (ParticleEventHandlerConstIterator it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it) {
			(*it)->_handle(particleTechnique, particle, timeElapsed);
		}
		mEventHandlersExecuted = true;
	}
	
	bool ParticleObserver::isParticleTypeToObserveSet() const {
		return mParticleTypeToObserveSet;
	}

} /* namespace ParticleUniverse */
