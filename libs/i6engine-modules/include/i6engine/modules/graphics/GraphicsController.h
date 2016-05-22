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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__
#define __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace modules {

	class GraphicsMailbox;
	class GraphicsManager;

	/**
	 * \class GraphicsController
	 * \brief This class handles the whole Ogre graphics subsystem
	 */
	class ISIXE_MODULES_API GraphicsController : public core::ModuleController {
	public:
		/**
		 * \brief Nothing to do yet
		 *
		 * This method has nothing to to do yet
		 */
		GraphicsController() : GraphicsController(nullptr) {
		}
		GraphicsController(HWND hWnd);

		/**
		 * \brief Nothing to do yet
		 *
		 * This method has nothing to do yet
		 */
		~GraphicsController();

	private:
		/**
		 * \brief
		 */
		GraphicsManager * _manager;

		/**
		 * \brief
		 */
		GraphicsMailbox * _mailbox;

		HWND _hWnd;

		/**
		 * \brief starts GraphicsManager
		 */
		void OnThreadStart() override;

		/**
		 * \brief Called when shutting down the graphics subsystem.
		 */
		void ShutDown() override;

		/**
		 * \brief Called on every tick of the graphics subsystem
		 *
		 *     In effect the main render-loop
		 *     invokes Ogre's _objRoot->renderOneFrame()
		 *			calls tick on GUIController and injectTimePulse on CEGUI
		 *
		 *			Updates the GUI and tells Ogre to render the scene.
		 */
		void Tick() override;

		/**
		 * \brief forbidden
		 */
		GraphicsController(const GraphicsController &) = delete;

		/**
		 * \brief forbidden
		 */
		const GraphicsController & operator=(const GraphicsController &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__ */

/*
 * @}
 */
