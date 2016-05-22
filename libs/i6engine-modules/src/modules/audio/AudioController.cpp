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

#include "i6engine/modules/audio/AudioController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/audio/AudioMailbox.h"
#include "i6engine/modules/audio/AudioManager.h"

namespace i6e {
namespace modules {

	AudioController::AudioController() : core::ModuleController(core::Subsystem::Audio), _mailbox(), _manager() {
	}

	AudioController::~AudioController() {
	}

	void AudioController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		_manager = new AudioManager();
		_mailbox = new AudioMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::AudioMessageType, AudioMailbox::News, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::AudioNodeMessageType, AudioMailbox::News, _mailbox);
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

} /* namespace modules */
} /* namespace i6e */
