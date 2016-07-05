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

#include "i6engine/modules/object/ObjectController.h"

#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"

#include "i6engine/modules/object/ObjectMailbox.h"
#include "i6engine/modules/object/ObjectManager.h"

namespace i6e {
namespace modules {

	ObjectController::ObjectController() : core::ModuleController(core::Subsystem::Object), _mailbox(), _manager() {
	}

	ObjectController::~ObjectController() {
	}

	void ObjectController::OnThreadStart() {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR
		i6eObjectFacade->registerNotifyCallback(boost::bind(&MessageSubscriber::notifyNewID, this, _1));
		i6eObjectFacade->registerDeletedNotifyCallback(boost::bind(&MessageSubscriber::notifyDeletedID, this, _1));

		_manager = new ObjectManager();
		_mailbox = new ObjectMailbox(_manager);

		ISIXE_REGISTERMESSAGETYPE(api::messages::ComponentMessageType, ObjectMailbox::NewsComponent, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectMessageType, ObjectMailbox::NewsObject, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::ObjectManagerMessageType, ObjectMailbox::NewsObjectManager, _mailbox);
		ISIXE_REGISTERMESSAGETYPE(api::messages::PositionMessageType, ObjectMailbox::NewsObject, _mailbox);

		i6eObjectFacade->setFrametime(getFrameTime());
	}

	void ObjectController::ShutDown() {
		ASSERT_THREAD_SAFETY_FUNCTION
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ComponentMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ObjectMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::ObjectManagerMessageType);
		ISIXE_UNREGISTERMESSAGETYPE(api::messages::PositionMessageType);

		ISIXE_LOG_INFO("ObjectController", "*** Shutting Down Subsystem ***");

		delete _mailbox;
		delete _manager;

		_mailbox = nullptr;
		_manager = nullptr;
	}

	void ObjectController::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION
		_manager->Tick();
	}

} /* namespace modules */
} /* namespace i6e */
