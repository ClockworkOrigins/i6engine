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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__
#define __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
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
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GRAPHICSCONTROLLER_H__ */

/*
 * @}
 */
