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

#include "i6engine/modules/graphics/GraphicsController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/graphics/GraphicsMailbox.h"
#include "i6engine/modules/graphics/GraphicsManager.h"

namespace i6engine {
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
} /* namespace i6engine */
