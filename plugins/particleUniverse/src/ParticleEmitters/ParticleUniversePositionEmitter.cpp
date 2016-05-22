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

#include "ParticleEmitters/ParticleUniversePositionEmitter.h"

namespace ParticleUniverse {

	// Constants
	const bool PositionEmitter::DEFAULT_RANDOMIZE = true;

	PositionEmitter::PositionEmitter() : ParticleEmitter(), mRandomized(DEFAULT_RANDOMIZE), mIndex(0) {
	}
	
	bool PositionEmitter::isRandomized() const {
		return mRandomized;
	}
	
	void PositionEmitter::setRandomized(bool randomized) {
		mRandomized = randomized;
	}
	
	const vector<Vector3> & PositionEmitter::getPositions() const {
		return mPositionList;
	}
	
	void PositionEmitter::addPosition(const Vector3 & pos) {
		mPositionList.push_back(pos);
	}
	
	void PositionEmitter::_notifyStart() {
		ParticleEmitter::_notifyStart();
		mIndex = 0;
	}
	
	void PositionEmitter::removeAllPositions() {
		mIndex = 0;
		mPositionList.clear();
	}
	
	unsigned short PositionEmitter::_calculateRequestedParticles(Real timeElapsed) {
		// Fast rejection
		if (mPositionList.empty()) {
			return 0;
		}

		if (mRandomized) {
			return ParticleEmitter::_calculateRequestedParticles(timeElapsed);
		} else if (mIndex < mPositionList.size()) {
			unsigned short requested = ParticleEmitter::_calculateRequestedParticles(timeElapsed);
			unsigned short size = static_cast<unsigned short>(mPositionList.size() - mIndex);
			if (requested > size) {
				return size;
			} else {
				return requested;
			}
		}

		return 0;
	}
	
	void PositionEmitter::_initParticlePosition(Particle * particle) {
		// Fast rejection
		if (mPositionList.empty()) {
			return;
		}

		/** Remark: Don't take the orientation of the node into account, because the positions shouldn't be affected by the rotated node.
		*/
		if (mRandomized) {
			size_t i = size_t(Math::UnitRandom() * (mPositionList.size() - 1));
			particle->position = getDerivedPosition() + _mEmitterScale * mPositionList[i];
		} else if (mIndex < mPositionList.size()) {
			particle->position = getDerivedPosition() + _mEmitterScale * mPositionList[mIndex];
			mIndex++;
		}

		particle->originalPosition = particle->position;
	}
	
	void PositionEmitter::copyAttributesTo(ParticleEmitter * emitter) {
		ParticleEmitter::copyAttributesTo(emitter);
		PositionEmitter * positionEmitter = static_cast<PositionEmitter *>(emitter);
		positionEmitter->mRandomized = mRandomized;
		positionEmitter->mPositionList = mPositionList;
	}

} /* namespace ParticleUniverse */
