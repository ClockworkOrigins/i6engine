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
 * \addtogroup Network
 * @{
 */

#ifndef __I6ENGINE_MODULES_NETWORKCONTROLLER_H__
#define __I6ENGINE_MODULES_NETWORKCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace modules {

	class NetworkMailbox;
	class NetworkManager;

	/**
	 * \class NetworkController
	 * \brief This class performs and provides various methods to the engine for the network
	 */
	class ISIXE_MODULES_API NetworkController : public core::ModuleController {
	public:
		/**
		 * \brief Nothing to do yet
		 * This method has nothing to to do yet
		 */
		NetworkController();

		/**
		 * \brief Nothing to do yet
		 * This method has nothing to do yet
		 */
		~NetworkController();

	private:
		NetworkMailbox * _mailbox;
		NetworkManager * _manager;

		/**
		 * \brief Called when starting the thread of the graphics subsystem.
		 * \details Registers with the messaging subsystem
		 */
		void OnThreadStart() override;

		/**
		 * \brief Called when shutting down the network subsystem.
		 * \details Cleans up all resources of the network subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief Called on every tick of the network subsystem.
		 * \details Updates the network
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		const NetworkController & operator=(const NetworkController &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkController(const NetworkController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_NETWORKCONTROLLER_H__ */

/**
 * @}
 */
