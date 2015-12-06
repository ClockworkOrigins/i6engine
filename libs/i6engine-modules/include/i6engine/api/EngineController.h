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

#ifndef __I6ENGINE_API_ENGINECONTROLLER_H__
#define __I6ENGINE_API_ENGINECONTROLLER_H__

#include <map>
#include <set>

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#include <tchar.h>
	#include <Windows.h>
#endif

#include "i6engine/utils/ExceptionQueue.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/core/configs/JobPriorities.h"

#include "boost/function.hpp"

#include "clockUtils/iniParser/iniParser.h"

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
	typedef void * HWND;
#endif

namespace i6engine {
namespace core {
	class EngineCoreController;
	class MessagingController;
	class ModuleController;
	class SubSystemController;
	enum class Subsystem;
} /* namespace core */
namespace api {

	class Application;
	class AudioFacade;
	class GraphicsFacade;
	class GUIFacade;
	class IDManager;
	class InputFacade;
	class LanguageManager;
	class MessagingFacade;
	class NetworkFacade;
	class ObjectFacade;
	class PhysicsFacade;
	class ScriptingFacade;
	class TextManager;
	class WaynetManager;

	enum class GameType {
		SINGLEPLAYER,
		CLIENT,
		SERVER
	};

	/**
	 * \brief This class is used as the Controller for the whole engine
	 * Create an instance of this class and use the functions to start / stop / ...
	 * the Engine
	 */
#ifdef ISIXE_WITH_LOGGING
	class ISIXE_MODULES_API EngineController : public utils::Singleton<EngineController, utils::Logger, utils::exceptions::ExceptionQueue> {
		friend class utils::Singleton<EngineController, utils::Logger, utils::exceptions::ExceptionQueue>;
#else
	class ISIXE_MODULES_API EngineController : public utils::Singleton<EngineController, utils::exceptions::ExceptionQueue> {
		friend class utils::Singleton<EngineController, utils::exceptions::ExceptionQueue>;
#endif

	public:
		/**
		 * \brief Destructor
		 */
		~EngineController();

		/**
		 * \brief This method registeres a modul
		 * \param[in] name name of the module
		 * \param[in] module the module to be registered
		 * \param[in] frameTime time between two ticks of this subsystem
		 * This function can be used to create a non default subsystem configurator
		 * or you can even register your own subsystems. But if you do this, make sure, they
		 * match the required behaviour.
		 * For most purposes, the default configuration is sufficiant and can be registered with
		 * \ref registerDefault()
		 */
		void registerSubSystem(const std::string & name, core::ModuleController * module, uint32_t frameTime);
		void registerSubSystem(const std::string & name, core::ModuleController * module, const std::set<core::Subsystem> & waitingFor);

		inline AudioFacade * getAudioFacade() const {
			return _audioFacade;
		}

		inline GraphicsFacade * getGraphicsFacade() const {
			return _graphicsFacade;
		}

		inline GUIFacade * getGUIFacade() const {
			return _guiFacade;
		}

		inline InputFacade * getInputFacade() const {
			return _inputFacade;
		}

		inline MessagingFacade * getMessagingFacade() const {
			return _messagingFacade;
		}

		inline NetworkFacade * getNetworkFacade() const {
			return _networkFacade;
		}

		inline ObjectFacade * getObjectFacade() const {
			return _objectFacade;
		}

		inline PhysicsFacade * getPhysicsFacade() const {
			return _physicsFacade;
		}

		inline ScriptingFacade * getScriptingFacade() const {
			return _scriptingFacade;
		}

		IDManager * getIDManager() const {
			return _idManager;
		}

		LanguageManager * getLanguageManager() const {
			return _languageManager;
		}

		TextManager * getTextManager() const {
			return _textManager;
		}

		WaynetManager * getWaynetManager() const {
			return _waynetManager;
		}

		/**
		 * \brief register an Application to the Engine
		 * \param[in] app reference to the application
		 */
		void registerApplication(Application & app);

		/**
		 * \brief A start method to Initialize and run the engine
		 */
		void start();

		inline void setType(GameType type) {
			_type = type;
		}

		/**
		 * \brief sets level of the debug drawer
		 */
		inline void setDebugdrawer(const uint8_t dd) {
			_debugdrawer = dd;
		}

		/**
		 * \brief gets level of the debug drawer
		 */
		inline uint8_t getDebugdrawer() const {
			return _debugdrawer;
		}

		/**
		 * \brief returns the registered Application
		 */
		inline Application * getAppl() const { return _appl; }

		/**
		 * \brief registers a basic default configuration
		 */
		void registerDefault(const bool ds) {
			registerDefault(ds, nullptr);
		}

		/**
		 * \brief registers a basic default configuration with a window handle
		 */
		void registerDefault(const bool ds, HWND hWnd);

		/**
		 * \brief call this method to shutdown everything
		 */
		void stop();

		inline core::EngineCoreController * getController() const {
			return _coreController;
		}

		/**
		 * \brief registers a timer
		 * \param[in] time time in microseconds until func is called
		 * \param[in] func function to be called after given time
		 * \param[in] looping defines whether method is looping or just running once
		 * \param[in] priority priority of this task, defining which Job will be handles preferred
		 */
		uint64_t registerTimer(uint64_t time, const boost::function<bool(void)> & func, bool looping, core::JobPriorities priority);

		/**
		 * \brief deletes all timer with given priority
		 */
		void removeTimer(core::JobPriorities priority);

		/**
		 * \brief deletes all timer with given name
		 */
		bool removeTimerID(uint64_t id);

		/**
		 * \brief returns the left time for the given timer ID in microseconds
		 */
		uint64_t getTimeLeft(uint64_t id) const;

		/**
		 * \brief returns current application time in microseconds
		 */
		uint64_t getCurrentTime() const;

		/**
		 * \brief sets the current time (only at clients, time will be synchronized by server)
		 */
		void setCurrentTime(uint64_t time);

		/**
		 * \brief returns the uuid of this node
		 */
		inline uint32_t getUUID() const {
			return _uuid;
		}

		/**
		 * \brief returns a new uuid
		 */
		uint32_t getNewUUID() const;

		/**
		 * \brief returns the iniParser that parsed the i6engine config file
		 */
		inline clockUtils::iniParser::IniParser & getIniParser() {
			return _iParser;
		}

		/**
		 * \brief returns the type of this node
		 */
		GameType getType() const {
			return _type;
		}

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		/**
		 * \brief creates a new Window
		 */
		HWND createWindow(HINSTANCE hInstance);
#endif

	private:
		std::map<std::string, std::pair<core::ModuleController *, uint32_t>> _queuedModules;
		std::map<std::string, std::pair<core::ModuleController *, std::set<core::Subsystem>>> _queuedModulesWaiting;
		core::SubSystemController * _subsystemController;
		core::EngineCoreController * _coreController;
		IDManager * _idManager;
		LanguageManager * _languageManager;
		TextManager * _textManager;
		WaynetManager * _waynetManager;
		Application * _appl;
		uint8_t _debugdrawer;

		AudioFacade * _audioFacade;
		GraphicsFacade * _graphicsFacade;
		GUIFacade * _guiFacade;
		InputFacade * _inputFacade;
		MessagingFacade * _messagingFacade;
		NetworkFacade * _networkFacade;
		ObjectFacade * _objectFacade;
		PhysicsFacade * _physicsFacade;
		ScriptingFacade * _scriptingFacade;

		core::MessagingController * _messagingController;

		uint32_t _uuid;

		/**
		 * \brief ini parser to load config file
		 */
		clockUtils::iniParser::IniParser _iParser;

		GameType _type;

		/**
		 * \brief Contructor
		 */
		EngineController();

		/**
		 * \brief starts the engine
		 * This function blocks. It will return only after the engine is completely shutdown
		 * You can now only interact with the engine from your application class.
		 */
		void runEngine();

		/**
		 * \brief shut down the engine
		 */
		void ShutDown();

		/**
		 * \brief forbidden
		 */
		EngineController(const EngineController &) = delete;

		/**
		 * \brief forbidden
		 */
		EngineController & operator=(const EngineController &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_ENGINECONTROLLER_H__ */

/**
 * @}
 */
