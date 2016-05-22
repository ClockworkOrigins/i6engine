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
 * \addtogroup PythonScripting
 * @{
 */

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/GameMessage.h"

namespace i6e {
namespace modules {

	class PythonScriptingController;
	class PythonScriptingManager;

	/**
	 * \class PythonScriptingMailbox
	 * \brief Handles the incoming messages on the scripting channel and calls the corresponding methods.
	 */
	class PythonScriptingMailbox {
		friend class PythonScriptingController;

	protected:
		/**
		 * \brief Constructor
		 */
		explicit PythonScriptingMailbox(PythonScriptingManager * m);

		/**
		 * \brief Destructor
		 */
		~PythonScriptingMailbox() {}

		/**
		 * \brief Handles incoming messages.
		 */
		void News(const api::GameMessage::Ptr & msg) const;

	private:
		PythonScriptingManager * _manager;

		/**
		 * \brief forbidden
		 */
		PythonScriptingMailbox(const PythonScriptingMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		PythonScriptingMailbox & operator=(const PythonScriptingMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGMAILBOX_H__ */

/**
 * @}
 */
