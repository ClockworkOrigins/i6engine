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

#ifndef __I6ENGINE_MODULES_NETWORKERRORS_H__
#define __I6ENGINE_MODULES_NETWORKERRORS_H__

#include <string>

#include "i6engine/utils/i6eThreadSafety.h"

namespace i6e {
namespace modules {

	class NetworkManager;

	class NetworkErrors {
		friend class NetworkManager;

	private:
		NetworkManager * _manager;

		/**
		 * \brief constructor taking NetworkController
		 */
		explicit NetworkErrors(NetworkManager * manager);

		/**
		 * \brief destructor
		 */
		~NetworkErrors();

		/**
		 * \brief called when connection to m2etis failed
		 */
		void connectionFailed(const std::string & other);

		/**
		 * \brief called when player leaves game
		 */
		void connectionClosed(const std::string & other);

		/**
		 * \brief called when address or port is already in use while connecting
		 */
		void bindFailure(const std::string & other);

		/**
		 * \brief forbidden
		 */
		NetworkErrors(const NetworkErrors &) = delete;

		/**
		 * \brief forbidden
		 */
		NetworkErrors & operator=(const NetworkErrors &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_NETWORKERRORS_H__ */

/**
 * @}
 */
