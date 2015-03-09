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

#include "i6engine/api/GameMessage.h"

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
		 *
		 */
		GraphicsController();

		/**
		 * \brief Nothing to do yet
		 *
		 * This method has nothing to do yet
		 *
		 */
		~GraphicsController();

		/**
		 * \brief starts GraphicsManager
		 */
		void OnThreadStart() override;

		/**
		 * \brief Handles incoming messages noticeable for the graphics subsystem.
		 * Adds Message to Message-Pipe by calling GraphicsMailbox::News(msg)
		 * \details Called by the global MessagingController .\n Delegates the received message to the GraMailbox.
		 * \attention Do not call this method explicitly. Only communicate with the graphics subsystem via the MessagingController .
		 * \param   const api::GameMessage::Ptr msg, objMessage Message Object
		 * \return   nothing
		 */
		void Mailbox(const api::GameMessage::Ptr & msg) const;

		/**
		 * \brief Called when shutting down the graphics subsystem.
		 *
		 *     Sends shutdown message informing GraphicsController shutdown to other objects via message pipe
		 * Sends a message in order to shutdown the Graphics Subsystem
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void ShutDown() override;

	protected:
		/**
		 * \brief Called on every tick of the graphics subsystem
		 *
		 *     In effect the main render-loop
		 *     invokes Ogre's _objRoot->renderOneFrame()
		 *			calls tick on GUIController and injectTimePulse on CEGUI
		 *
		 *			Updates the GUI and tells Ogre to render the scene.
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void Tick() override;

	private:
		/**
		 * \brief
		 */
		GraphicsManager * _manager;

		/**
		 * \brief
		 */
		GraphicsMailbox * _mailbox;

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
