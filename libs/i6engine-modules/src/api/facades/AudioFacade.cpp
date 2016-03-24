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

#include "i6engine/api/facades/AudioFacade.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/AudioConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	AudioFacade::AudioFacade() : _handleCounter(0) {
	}

	AudioFacade::~AudioFacade() {
	}

	void AudioFacade::createNode(int64_t comId, const std::string & f, bool l, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Create, new audio::Audio_Node_Create(comId, f, l, m, p, d, cacheable, category), core::Subsystem::Unknown));
	}

	void AudioFacade::deleteNode(int64_t comId) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioNodeMessageType, audio::AudioNode, core::Method::Delete, new audio::Audio_Node_Delete(comId), core::Subsystem::Unknown));
	}

	void AudioFacade::updateListener(const Vec3 & position, const Quaternion & rotation, const Vec3 & velocity) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioListener, core::Method::Update, new audio::Audio_Listener_Update(position, rotation, velocity), core::Subsystem::Unknown));
	}

	void AudioFacade::updatePosition(int64_t comId, const Vec3 & position) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioPosition, core::Method::Update, new audio::Audio_Position_Update(comId, position), core::Subsystem::Unknown));
	}

	audio::SoundHandle AudioFacade::playSound(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category) {
		audio::SoundHandle handle = _handleCounter++;
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioPlaySound, core::Method::Create, new audio::Audio_PlaySound_Create(handle, f, m, p, d, cacheable, category), core::Subsystem::Unknown));
		return handle;
	}

	audio::SoundHandle AudioFacade::playSoundWithCallback(const std::string & f, double m, const Vec3 & p, const Vec3 & d, bool cacheable, const std::string & category, const std::function<void(bool)> callback) {
		uint64_t handle = _handleCounter++;
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioPlaySoundWithCallback, core::Method::Create, new audio::Audio_PlaySoundWithCallback_Create(handle, f, m, p, d, cacheable, category, callback), core::Subsystem::Unknown));
		return handle;
	}

	void AudioFacade::stopSound(audio::SoundHandle handle) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioStopSound, core::Method::Delete, new audio::Audio_StopSound_Delete(handle), core::Subsystem::Unknown));
	}

	void AudioFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::AudioMessageType, audio::AudioReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6engine */
