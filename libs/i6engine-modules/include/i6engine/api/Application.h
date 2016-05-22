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
 * \addtogroup API
 * @{
 */

#ifndef __I6ENGINE_API_APPLICATION_H__
#define __I6ENGINE_API_APPLICATION_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/subsystem/ModuleController.h"

namespace i6e {
namespace core {
	class IPKey;
} /* namespace core */
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
		Application() : core::ModuleController(core::Subsystem::Application), _windowName("Default") {}

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
		virtual void disconnectCallback(const core::IPKey &) {}

		/**
		 * \brief called when connecting to server failes
		 */
		virtual void connectionFailed(const core::IPKey &) {}

		/**
		 * \brief called when address or port is already in use while connecting to m2etis
		 */
		virtual void bindFailure() {}

		/**
		 * \brief called when connection request was handled
		 * \param[in] key identifying the user who joined
		 */
		virtual void connected(const core::IPKey &) {}

		/**
		 * \brief called from engine to signal shutdown
		 */
		void ShutDown() {
		}

	protected:
		ASSERT_THREAD_SAFETY_HEADER

	private:
		std::string _windowName; //!< name of the application being used as the window title

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
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_APPLICATION_H__ */

/**
 * @}
 */
