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

#include "i6engine/rpg/npc/queueJobs/SayJob.h"

#include "i6engine/utils/i6eRandom.h"
#include "i6engine/utils/i6eString.h"

#include "i6engine/math/i6eMath.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/components/PhysicalStateComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/config/ExternalConstants.h"
#include "i6engine/rpg/dialog/DialogManager.h"
#include "i6engine/rpg/npc/NPC.h"
#include "i6engine/rpg/npc/queueJobs/WaitSayJob.h"

namespace i6e {
namespace rpg {
namespace npc {

	SayJob::SayJob(NPC * self, const std::string & soundKey, const std::string & subtitleKey, const std::vector<WaitSayJob *> & jobs) : api::MessageSubscriberFacade(), _self(self), _soundKey(soundKey), _subtitleKey(subtitleKey), _jobs(jobs), _startTime(), _subtitleDuration(), _soundFound(true), _soundFinished(false), _soundHandle() {
	}

	void SayJob::start() {
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, SayJob::News, this);
		// print subtitle
		_startTime = api::EngineController::GetSingleton().getCurrentTime();
		std::string subtitleText = api::EngineController::GetSingleton().getTextManager()->getText(_subtitleKey);

		if (dialog::DialogManager::GetSingleton().getSubtitlesEnabled()) {
			_subtitleDuration = uint64_t(utils::split(subtitleText, " ").size() * config::SUBTITLE_TIME_PER_WORD) * 1000000;

			api::EngineController::GetSingleton().getGUIFacade()->setText("SubtitleWidget", subtitleText);
			api::EngineController::GetSingleton().getGUIFacade()->setVisibility("SubtitleWidget", true);
		}

		// start sound
		auto psc = _self->getGO()->getGOC<api::PhysicalStateComponent>(api::components::ComponentTypes::PhysicalStateComponent);
		_soundHandle = api::EngineController::GetSingleton().getAudioFacade()->playSoundWithCallback(api::EngineController::GetSingleton().getTextManager()->getText(_soundKey), config::DIALOG_MAX_DISTANCE, psc->getPosition(), math::rotateVector(Vec3(0.0, 0.0, 1.0), psc->getRotation()), false, "DIALOG", [this](bool b) {
			if (b) {
				_soundFinished = true;
			} else {
				_soundFound = false;
			}
		});

		i6e::api::objects::GOTemplate tmpl;
		i6e::api::attributeMap params;
		Vec3 pos = psc->getPosition() + Vec3(0.0, 1.0, 0.0) + Vec3(0.0, utils::Random::GetSingleton().rand(101) / 100.0, 0.0) + math::rotateVector(Vec3(0.0, 0.0, -5.0), psc->getRotation() * Quaternion(Vec3(0.0, 1.0, 0.0), (-45 + int(utils::Random::GetSingleton().rand(91))) * PI / 180.0));
		pos.insertInMap("pos", params);
		psc->getPosition().insertInMap("lookAt", params);

		tmpl._components.push_back(i6e::api::objects::GOTemplateComponent("Camera", params, "", false, false));
		api::EngineController::GetSingleton().getObjectFacade()->createObject("DialogCam", tmpl, api::EngineController::GetSingleton().getUUID(), false);
	}

	void SayJob::loop() {
		processMessages();
	}

	void SayJob::finish() {
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		api::EngineController::GetSingleton().getObjectFacade()->deleteAllObjectsOfType("DialogCam");
		for (WaitSayJob * j : _jobs) {
			j->setFinished(true);
		}
		api::EngineController::GetSingleton().getGUIFacade()->setVisibility("SubtitleWidget", false);
	}

	bool SayJob::condition() {
		return (_soundFound && _soundFinished) || (!_soundFound && api::EngineController::GetSingleton().getCurrentTime() - _startTime >= _subtitleDuration);
	}

	void SayJob::News(const api::GameMessage::Ptr & msg) {
		if (msg->getMessageType() == api::messages::InputMessageType) {
			if (msg->getSubtype() == api::keyboard::KeyboardMessageTypes::KeyKeyboard) {
				api::KeyCode kc = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->code;
				api::KeyState ks = dynamic_cast<api::input::Input_Keyboard_Update *>(msg->getContent())->pressed;
				if (kc == api::KeyCode::KC_ESCAPE && ks == api::KeyState::KEY_PRESSED) {
					api::EngineController::GetSingleton().getAudioFacade()->stopSound(_soundHandle);
					_soundFound = false;
					_subtitleDuration = 0;
				}
			}
		}
	}

} /* namespace npc */
} /* namespace rpg */
} /* namespace i6e */
