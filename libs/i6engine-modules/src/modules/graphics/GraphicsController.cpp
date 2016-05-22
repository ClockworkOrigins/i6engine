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

#include "i6engine/modules/graphics/GraphicsController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/graphics/GraphicsMailbox.h"
#include "i6engine/modules/graphics/GraphicsManager.h"

namespace i6e {
namespace modules {

	GraphicsController::GraphicsController(HWND hWnd) : core::ModuleController(core::Subsystem::Graphic), _manager(nullptr), _mailbox(nullptr), _hWnd(hWnd) {
	}

	GraphicsController::~GraphicsController() {
	}

	void GraphicsController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		api::EngineController::GetSingletonPtr()->getGraphicsFacade()->registerNotifyCallback(boost::bind(&MessageSubscriber::notifyNewID, this, _1));

		_manager = new GraphicsManager(this, _hWnd);
		_mailbox = new GraphicsMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::GraphicsMessageType, GraphicsMailbox::News, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::GraphicsNodeMessageType, GraphicsMailbox::News, _mailbox);
	}

	void GraphicsController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION

		ISIXE_UNREGISTERMESSAGETYPE(api::messages::GraphicsMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::GraphicsNodeMessageType);

		ISIXE_LOG_INFO("GraphicsController", "*** Shutting Down Subsystem ***");

		delete _mailbox;
		_mailbox = nullptr;
		delete _manager;
		_manager = nullptr;
	}

	void GraphicsController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6e */
