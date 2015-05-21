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

#include "i6engine/rpg/npc/queueJobs/SayJob.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/queueJobs/WaitSayJob.h"

namespace i6engine {
namespace rpg {
namespace npc {

	SayJob::SayJob(NPC * self, const std::string & soundKey, const std::string & subtitleKey, const std::vector<WaitSayJob *> & jobs) : _self(self), _soundKey(soundKey), _subtitleKey(subtitleKey), _jobs(jobs), _startTime(), _subtitleDuration(), _soundFound(true), _soundFinished(false) {
	}

	void SayJob::start() {
		// print subtitle
		_startTime = api::EngineController::GetSingleton().getCurrentTime();
		std::string subtitleText = api::EngineController::GetSingleton().getTextManager()->getText(_subtitleKey);

		_subtitleDuration = uint64_t(subtitleText.size() * config::SUBTITLE_TIME_PER_CHARACTER);

		api::EngineController::GetSingleton().getGUIFacade()->setText("SubtitleWidget", subtitleText);

		// start sound
		auto psc = _self->getGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(api::EngineController::GetSingleton().getTextManager()->getText(_soundKey), config::DIALOG_MAX_DISTANCE, psc->getPosition(), math::rotateVector(Vec3(0.0, 0.0, 1.0), psc->getRotation()), false, [this](bool b) {
			if (b) {
				_soundFinished = true;
			} else {
				_soundFound = false;
			}
		});
	}

	void SayJob::loop() {
	}

	void SayJob::finish() {
		for (WaitSayJob * j : _jobs) {
			j->setFinished(true);
		}
	}

	bool SayJob::condition() {
		return (_soundFound && _soundFinished) || (!_soundFound && api::EngineController::GetSingleton().getCurrentTime() - _startTime >= _subtitleDuration);
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6engine */
