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
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_APPLICATION_H__
#define __I6ENGINE_API_APPLICATION_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6engine {
namespace core {
	class IPKey;
}
namespace api {

	class EngineController;

	/**
	 * \brief Interface for the game developer
	 *
	 * these functions are callbacks that are called by the Engine in appropriate situations
	 * derive from this class to get your main application
	 * Override all pure virtual methods and all virtual methods you need. You can safely assume, simply overriding 
	 * won't cause problems, as the functions don't do anything by default.
	 */
	class ISIXE_MODULES_API Application : public core::ModuleController {
	public:
		Application() : core::ModuleController(core::Subsystem::Unknown), _windowName("Default") {}

		virtual ~Application() {}

		/**
		 * \brief called on EngineStart
		 *
		 * In the current state, only the very basic objects are initialised.
		 * You probably don't want to do anything here
		 */
		virtual void Initialize() = 0;

		/**
		 * \brief called after finishing initialization
		 *
		 * Now everthing is loaded, initialised and ready to be run
		 * You can do what ever you want now.
		 */
		virtual void AfterInitialize() = 0;

		/**
		 * \brief called in every loop step
		 *
		 * This function is called whenever the mainLoop starts from the beginning.<br>
		 * After returning from this function the world is updated and afterwards rendered.
		 */
		virtual void Tick() = 0;

		/**
		 * \brief called on EngineShutdown
		 *
		 * This function is called just before the Engine starts shutting down.<br>
		 * In this state, the game is still entirely present, but frozen. Thus you have atomic access to the whole game.
		 * This function can be used for saving e.g., but keep in mind, the engine is paused. Messages, Timers etc. won't be processed.
		 */
		virtual bool ShutdownRequest() = 0;

		/**
		 * \brief called after everything is completely shutdown
		 *
		 * after returning from this function, the engine just returns.<br>
		 * here you can free all ressources before the game object is destroyed.
		 */
		virtual void Finalize() = 0;

		/**
		 * \brief returns the name of the Application
		 * \returns Name of the game
		 */
		std::string getName() const {
			return _windowName;
		}

		/**
		 * \brief sets the name of the Application
		 * \param[in] name name that will be used as the window title
		 */
		void setName(const std::string & name) {
			_windowName = name;
		}

		/**
		 * \brief called when a player had a disconnect
		 * \param[in] key key identifying the player
		 */
		virtual void disconnectCallback(const core::IPKey & key) {}

		/**
		 * \brief called when connecting to server failes
		 */
		virtual void connectionFailed(const i6engine::core::IPKey & s) {}

		/**
		 * \brief called when address or port is already in use while connecting to m2etis
		 */
		virtual void bindFailure() {}

		/**
		 * \brief called when connection request was handled
		 * \param[in] key identifying the user who joined
		 */
		virtual void connected(const core::IPKey & key) {}

		/**
		 * \brief called from engine to signal shutdown
		 */
		void ShutDown() {
		}

	protected:
		ASSERT_THREAD_SAFETY_HEADER

	private:
		/**
		 * \brief called after the thread was created
		 */
		void OnThreadStart() {
			ASSERT_THREAD_SAFETY_CONSTRUCTOR
		}

		/**
		 * \brief forbidden
		 */
		Application(const Application &) = delete;

		/**
		 * \brief forbidden
		 */
		Application & operator=(const Application &) = delete;

		std::string _windowName; //!< name of the application being used as the window title
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_APPLICATION_H__ */

/**
 * @}
 */
