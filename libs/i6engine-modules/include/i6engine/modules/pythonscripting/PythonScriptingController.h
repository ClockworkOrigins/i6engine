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

#ifndef __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__
#define __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace modules {

	class PythonScriptingMailbox;
	class PythonScriptingManager;

	/**
	 * \class PythonScriptingController
	 * \brief Controls the scripting
	 *
	 * This class creates the scripting and manages its objects.
	 */
	class ISIXE_MODULES_API PythonScriptingController : public core::ModuleController {
	public:
		/**
		 * \brief constructor
		 */
		PythonScriptingController();

		/**
		 * \brief destructor
		 */
		~PythonScriptingController();

	private:
		PythonScriptingMailbox * _mailbox;
		PythonScriptingManager * _manager;

		/**
		 * \brief This method will be called when starting the thread of the subsystem.
		 */
		void OnThreadStart() override;

		/**
		 * \brief This method will be called when shutting down the subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief This method will be called in every tick of the subsystem.
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		PythonScriptingController(const PythonScriptingController &) = delete;

		/**
		 * \brief forbidden
		 */
		const PythonScriptingController & operator=(const PythonScriptingController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_PYTHONSCRIPTINGCONTROLLER_H__ */

/**
 * @}
 */
