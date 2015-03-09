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

#include "i6engine/modules/audio/AudioController.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/audio/AudioMailbox.h"
#include "i6engine/modules/audio/AudioManager.h"

namespace i6engine {
namespace modules {

	AudioController::AudioController() : core::ModuleController(core::Subsystem::Audio), _mailbox(), _manager() {
	}

	AudioController::~AudioController() {
	}

	void AudioController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		ISIXE_REGISTERMESSAGETYPE(api::messages::AudioMessageType, AudioController::Mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::AudioNodeMessageType, AudioController::Mailbox);

		_manager = new AudioManager();
		_mailbox = new AudioMailbox(_manager);
	}

	void AudioController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_LOG_INFO("AudioController", "***Shutting down subsystem***");

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::AudioMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::AudioNodeMessageType);
		delete _manager;
		delete _mailbox;
	}

	void AudioController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

	void AudioController::Mailbox(const api::GameMessage::Ptr & msg) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		_mailbox->News(msg);
	}

} /* namespace modules */
} /* namespace i6engine */
