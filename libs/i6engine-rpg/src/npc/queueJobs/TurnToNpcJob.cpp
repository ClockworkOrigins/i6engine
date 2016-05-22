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

#include "i6engine/rpg/npc/queueJobs/TurnToNpcJob.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/objects/GameObject.h"

namespace i6e {
namespace rpg {
namespace npc {

	TurnToNpcJob::TurnToNpcJob(const api::WeakGOPtr & self, const api::WeakGOPtr & target) : _self(self), _target(target) {
	}

	void TurnToNpcJob::start() {
	}

	void TurnToNpcJob::loop() {
		auto selfPSC = _self.get()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		auto targetPSC = _target.get()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);

		Vec3 selfDir = math::rotateVector(Vec3(0.0, 0.0, 1.0), selfPSC->getRotation());
		Vec3 targetDir = targetPSC->getPosition() - selfPSC->getPosition();

		double angle = Vec3::crossAngleSigned(selfDir, targetDir, Vec3(0.0, 1.0, 0.0));

		if (std::abs(angle - PI) < PI / 90) {
		} else {
			if (angle > 0) {
				selfPSC->applyRotation(Quaternion(Vec3(0.0, 1.0, 0.0), -PI / 45));
			} else {
				selfPSC->applyRotation(Quaternion(Vec3(0.0, 1.0, 0.0), PI / 45));
			}
		}
	}

	void TurnToNpcJob::finish() {
	}

	bool TurnToNpcJob::condition() {
		auto selfPSC = _self.get()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		auto targetPSC = _target.get()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);

		Vec3 selfDir = math::rotateVector(Vec3(0.0, 0.0, 1.0), selfPSC->getRotation());
		Vec3 targetDir = targetPSC->getPosition() - selfPSC->getPosition();

		double angle = Vec3::crossAngle(selfDir, targetDir);

		// maximum abberation is 10 degree
		return std::abs(angle - PI) < PI / 90;
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
