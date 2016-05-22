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

#include "ParticleUniverseAttachable.h"

namespace ParticleUniverse {

	// Constants
	String Attachable::PU_ATTACHABLE = "PUAttachable";

	Attachable::Attachable() : Extern(), MovableObject(), mAABB(), mDistanceThreshold(std::numeric_limits<float>::max()), mDistanceThresholdSet(false) {
		// The Movableobject name is auto generated to prevent a clash in Ogre
		mBoundingRadius = 1;
		std::stringstream ss; 
		ss << this;
		MovableObject::mName = "PUExtern" + ss.str();
	}

	void Attachable::_notifyAttached(Ogre::Node * parent, bool isTagPoint) {
		MovableObject::_notifyAttached(parent, isTagPoint);
	}

	void Attachable::_notifyCurrentCamera(Camera * cam) {
		Ogre::MovableObject::_notifyCurrentCamera(cam);
	}

	const String & Attachable::getMovableType() const {
		return PU_ATTACHABLE;
	}

	const AxisAlignedBox & Attachable::getBoundingBox() const {
		return mAABB;
	}

	Real Attachable::getBoundingRadius() const {
		return mBoundingRadius;
	}

	Real Attachable::getDistanceThreshold() const {
		return mDistanceThreshold;
	}

	void Attachable::setDistanceThreshold(const Real distanceThreshold) {
		mDistanceThresholdSet = true;
		mDistanceThreshold = distanceThreshold;
	}

	void Attachable::_updateRenderQueue(Ogre::RenderQueue * queue) {
		// Nothing to do here.
	}

	void Attachable::copyAttributesTo(Extern * externObject) {
		Extern::copyAttributesTo(externObject);
		Attachable * attachable = static_cast<Attachable *>(externObject);
		attachable->setDistanceThreshold(mDistanceThreshold);
	}

	void Attachable::_prepare(ParticleTechnique * technique) {
		// Nothing to do here.
	}

	void Attachable::_unprepare(ParticleTechnique * technique) {
		// Nothing to do here.
	}

	void Attachable::_interface(ParticleTechnique * technique, Particle * particle, Real timeElapsed) {
		// Nothing to do here.
	}

} /* namespace ParticleUniverse */
