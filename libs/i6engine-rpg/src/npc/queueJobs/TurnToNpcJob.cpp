/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
