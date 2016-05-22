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
