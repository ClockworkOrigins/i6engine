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

/**
 * \addtogroup Object
 * @{
 */

#ifndef __I6ENGINE_MODULES_OBJECTCONTROLLER_H__
#define __I6ENGINE_MODULES_OBJECTCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace api {
	class ObjectFacade;
} /* namespace api */

namespace modules {

	class ObjectManager;
	class ObjectMailbox;

	/**
	 * \class ObjectController
	 * \brief It administers all GameObjects and their GOComponents.
	 *
	 * Have a look at \link i6engine here \endlink for an overview of typedef's in namespace %i6engine.
	 */
	class ISIXE_MODULES_API ObjectController : public core::ModuleController {
	public:
		/**
		 * \brief Constructor for ObjectController.
		 */
		ObjectController();

		/**
		 * \brief destructor
		 */
		~ObjectController();

	private:
		ObjectMailbox * _mailbox;

		ObjectManager * _manager;

		/**
		 * \brief This method will be called when starting the thread of ObjectController.
		 * The ObjectController subscribes itself on the Messaging System on start.
		 */
		void OnThreadStart() override;

		/**
		 * \brief This method will be called in every tick of ObjectController.
		 * It processes all messages for the GOComponents.
		 */
		void Tick() override;

		/**
		 * \brief This method will be called when shutting down ObjectController.
		 * For every GameObject in _GOList DeleteObject will be called.
		 */
		void ShutDown() override;

		/**
		 * \brief forbidden
		 */
		ObjectController(const ObjectController &) = delete;

		/**
		 * \brief forbidden
		 */
		ObjectController & operator=(const ObjectController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_OBJECTCONTROLLER_H__ */

/**
 * @}
 */
