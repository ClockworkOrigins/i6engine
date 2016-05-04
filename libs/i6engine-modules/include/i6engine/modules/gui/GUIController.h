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

/**
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUICONTROLLER_H__
#define __I6ENGINE_MODULES_GUICONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GUIMailbox;
	class GUIManager;

	/**
	 * \class GUIController
	 * \brief Responsible for initializing and updating the gui.
	 * \attention Do NOT call any of its method directly. Communicate with the gui subsystem via the MessagingController only! Otherwise you will see unexpected behavior (e.g segmentation faults)!
	 */
	class GUIController: public core::ModuleController {
		friend class GraphicsManager;

	public:
		/**
		 * \brief constructor
		 */
		GUIController();

		/**
		 * \brief destructor
		 */
		~GUIController();

	private:
		GUIManager * _manager;
		GUIMailbox * _mailbox;

		/**
		 * \brief Called on startup of the gui subsystem.
		 *
		 *     calls initialize GUI and registers with message pipe
		 *
		 * Currently used as a wrapper to delegate the initialziation of the gui subsystem.
		 * If a thread safe gui library is used in the future, this method will be called automatically by the SubSystemController when starting the gui subsytem.
		 */
		void OnThreadStart() override;

		 /**
		 * \brief This method will be called when shutting down the subsystem.
		 *
		 *     shuts down by calling CEGUI's shut down functions
		 *
		 *			Will destroy all gui elements and the CEGUI rendering system. Leaves Log_Info with its steps.
		 */
		void ShutDown() override;

		/**
		 * \brief This method will be called in every tick of the subsystem.
		 *
		 *     tells core to process Messages
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		GUIController(const GUIController &) = delete;

		/**
		 * \brief forbidden
		 */
		const GUIController & operator=(const GUIController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUICONTROLLER_H__ */

/**
 * @}
 */
