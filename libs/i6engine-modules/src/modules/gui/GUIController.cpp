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

#include "i6engine/modules/gui/GUIController.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"

#include "i6engine/modules/gui/GUIMailbox.h"
#include "i6engine/modules/gui/GUIManager.h"

namespace i6e {
namespace modules {

	GUIController::GUIController() : core::ModuleController(core::Subsystem::GUI), _manager(nullptr), _mailbox(nullptr) {
	}

	GUIController::~GUIController() {
	}

	void GUIController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		_manager = new GUIManager(this);
		_mailbox = new GUIMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::GUIMessageType, GUIMailbox::News, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::InputMessageType, GUIMailbox::News, _mailbox);
	}

	void GUIController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		try {
			core::MessageSubscriber::processMessages();

			_manager->Tick();
		} catch(utils::exceptions::SubsystemException & e) {
			e.writeLog();
			// check errorcode and try to repair, if not possible:
			e.PassToMain();
		}
	}

	void GUIController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::GUIMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::InputMessageType);
		ISIXE_LOG_INFO("GuiController", "*** Shutting Down Subsystem ***");

		delete _mailbox;
		_mailbox = nullptr;
		delete _manager;
		_manager = nullptr;
	}

} /* namespace modules */
} /* namespace i6e */
