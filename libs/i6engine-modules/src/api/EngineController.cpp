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

#include "i6engine/api/EngineController.h"

#include <csignal>

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
	#include <WinSock2.h>
#endif

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/core/EngineCoreController.h"
#include "i6engine/core/messaging/MessagingController.h"
#include "i6engine/core/subsystem/ModuleController.h"
#include "i6engine/core/subsystem/SubSystemController.h"

#include "i6engine/configs/FrameTimes.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/AudioFacade.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/InputFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include "i6engine/api/facades/NetworkFacade.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/facades/PhysicsFacade.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/TextManager.h"

#ifdef ISIXE_WITH_AUDIO
	#include "i6engine/modules/audio/AudioController.h"
#endif

#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/input/InputController.h"
#include "i6engine/api/manager/IDManager.h"

#ifdef ISIXE_NETWORK
	#include "i6engine/modules/network/NetworkController.h"
#endif

#include "i6engine/modules/object/ObjectController.h"
#include "i6engine/modules/physics/PhysicsController.h"

#ifdef ISIXE_SCRIPTING
	#include "i6engine/modules/scripting/ScriptingController.h"
#endif

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"

// Ensures that X11 key repeat is reset to original value in case of a application crash.
void forceCleanup(int param) {
	ISIXE_LOG_WARN("EngineController", "***Dirty shutdown detected: running forced cleanup to reset X11 key repeat rate***");
	i6engine::api::EngineController::GetSingletonPtr()->stop();
}

namespace i6engine {
namespace api {

	EngineController::EngineController() : _queuedModules(), _queuedModulesWaiting(), _subsystemController(new core::SubSystemController()), _coreController(new core::EngineCoreController(_subsystemController)), _idManager(new IDManager()), _textManager(new TextManager()), _appl(), _debugdrawer(0), _ds(false), _audioFacade(new AudioFacade()), _graphicsFacade(new GraphicsFacade()), _guiFacade(new GUIFacade()), _inputFacade(new InputFacade()), _messagingFacade(new MessagingFacade()), _networkFacade(new NetworkFacade()), _objectFacade(new ObjectFacade()), _physicsFacade(new PhysicsFacade()), _scriptingFacade(new ScriptingFacade()), _messagingController(new core::MessagingController()), _uuid(getNewUUID()), _iParser(), _type(GameType::SINGLEPLAYER) {
		// WORKAROUND: Install signal handlers to overcome OIS's limitation to handle X11 key repeat rate properly when crashing.
		signal(SIGINT, forceCleanup);
		// TODO: kA
		// aber ich will wissen was schief geht und v.a. wo!!!
		// signal(SIGSEGV, forceCleanup);
		signal(SIGTERM, forceCleanup);

		_subsystemController->registerController(_coreController);

		_messagingFacade->registerMessagingController(_messagingController);

		if (clockUtils::ClockError::SUCCESS != _iParser.load("i6engine.ini")) {
			ISIXE_THROW_FAILURE("EngineController", "An exception has occurred: i6engine.ini couldn't be parsed!");
		}

		std::string textDir;
		if (clockUtils::ClockError::SUCCESS != _iParser.getValue("GENERAL", "i6engineTextDir", textDir)) {
			ISIXE_THROW_FAILURE("EngineController", "An exception has occurred: i6engine.ini couldn't be parsed!");
		}

		_textManager->initialize(textDir);
	}

	EngineController::~EngineController() {
		delete _subsystemController;
		delete _coreController;
		delete _idManager;
		delete _textManager;

		for (const std::pair<std::string, std::pair<core::ModuleController *, uint32_t>> & modulesPair : _queuedModules) {
			delete modulesPair.second.first;
		}
		for (const std::pair<std::string, std::pair<core::ModuleController *, std::set<core::Subsystem>>> & modulesPair : _queuedModulesWaiting) {
			delete modulesPair.second.first;
		}

		_queuedModules.clear();

		delete _audioFacade;
		delete _graphicsFacade;
		delete _guiFacade;
		delete _inputFacade;
		delete _messagingFacade;
		delete _networkFacade;
		delete _objectFacade;
		delete _physicsFacade;
		delete _scriptingFacade;

		delete _messagingController;
	}

	void EngineController::runEngine() {
		for (const std::pair<std::string, std::pair<core::ModuleController *, uint32_t>> & modulesPair : _queuedModules) {
			_subsystemController->QueueSubSystemStart(modulesPair.second.first, modulesPair.second.second);
		}
		for (const std::pair<std::string, std::pair<core::ModuleController *, std::set<core::Subsystem>>> & modulesPair : _queuedModulesWaiting) {
			_subsystemController->QueueSubSystemStart(modulesPair.second.first, modulesPair.second.second);
		}

		_subsystemController->QueueSubSystemStart(_appl, LNG_GAME_FRAME_TIME);

		volatile bool run = true;

		// need 'this' captured to compile with gcc 4.7...
		boost::thread thrd([this, &run]() {
			// read stdin
			std::string str;

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_LINUX
			struct timeval tv;

			while (run) {
				fd_set readset;
				FD_ZERO(&readset);
				FD_SET(0, &readset);
				tv.tv_sec = 0;
				tv.tv_usec = 10000;
				int result = select(1, &readset, NULL, NULL, &tv);
				if (result > 0 && FD_ISSET(0, &readset)) {
					if (std::cin.peek() != -1) {
						std::getline(std::cin, str);
						GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GameMessageType, input::InputConsoleRead, core::Method::Create, new input::Input_ConsoleRead_Create(str), core::Subsystem::Unknown);
						EngineController::getMessagingFacade()->deliverMessage(msg);
					}
				}
			}
#endif
		});

		_coreController->RunEngine();
		run = false;
		thrd.join();
	}

	void EngineController::registerSubSystem(const std::string & name, core::ModuleController * module, uint32_t frameTime) {
		_queuedModules.insert(std::make_pair(name, std::make_pair(module, frameTime)));
		module->setController(_subsystemController, _coreController, _messagingController);
	}

	void EngineController::registerSubSystem(const std::string & name, core::ModuleController * module, const std::set<core::Subsystem> & waitingFor) {
		_queuedModulesWaiting.insert(std::make_pair(name, std::make_pair(module, waitingFor)));
		module->setController(_subsystemController, _coreController, _messagingController);
	}

	void EngineController::start(const bool ds) {
		// Call the user-defined function Initialize (Application.h)
		if (ds == true) {
			ISIXE_LOG_INFO("EngineController", "Starting, Server is enabled");
		} else {
			ISIXE_LOG_INFO("EngineController", "Starting, Server is disabled");
		}
		_ds = ds;
		_appl->Initialize();
		_coreController->SetOnAfterInitializedCallback(boost::bind(&Application::AfterInitialize, _appl));
		runEngine();
	}

	void EngineController::ShutDown() {
		// ATTENTION: dont' call this function to stop the game. Use stop() instead. otherwise engine won't shutdown completely
		// TODO (???): game should be paused than call _appl->Shutdown, than shutdown
		// thus the GameDeveloper can just focus on PreShutdown actions without handeling a changing world
		// maybe keep Messaging/Networking-Thread running?
		if (_appl->ShutdownRequest()) {
			_coreController->ShutDown();

			while (!_subsystemController->isShutdownComplete()) {
				boost::this_thread::sleep(boost::posix_time::milliseconds(10));
			}

			_appl->Finalize();
		}
	}

	void EngineController::registerApplication(Application & app) {
		_appl = &app;
		_appl->setController(_subsystemController, _coreController, _messagingController);
	}

	void EngineController::registerDefault(const bool ds) {
		_ds = ds;

		if (!_ds) {
#ifdef ISIXE_NETWORK
			registerSubSystem("Network", new modules::NetworkController(), LNG_NETWORK_FRAME_TIME);
#endif
			registerSubSystem("Graphics", new modules::GraphicsController(), { i6engine::core::Subsystem::Object });
			registerSubSystem("Object", new modules::ObjectController(), LNG_OBJECT_FRAME_TIME);
			registerSubSystem("Input", new modules::InputController(), LNG_INPUT_FRAME_TIME);
			registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#ifdef ISIXE_SCRIPTING
			registerSubSystem("Scripting", new modules::ScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#endif
#ifdef ISIXE_WITH_AUDIO
			registerSubSystem("Audio", new modules::AudioController(), LNG_SCRIPTING_FRAME_TIME);
#endif
		} else {
#ifdef ISIXE_NETWORK
			registerSubSystem("Network", new modules::NetworkController(), LNG_NETWORK_FRAME_TIME);
#endif
			registerSubSystem("Object", new modules::ObjectController(), LNG_OBJECT_FRAME_TIME);
			registerSubSystem("Physics", new modules::PhysicsController(), LNG_PHYSICS_FRAME_TIME);
#ifdef ISIXE_SCRIPTING
			registerSubSystem("Scripting", new modules::ScriptingController(), LNG_SCRIPTING_FRAME_TIME);
#endif
		}
	}

	uint64_t EngineController::registerTimer(uint64_t time, const boost::function<bool(void)> & func, bool looping, uint16_t priority) {
		return _coreController->registerTimer(time, func, looping, priority);
	}

	void EngineController::removeTimer(uint16_t priority) {
		_coreController->removeTimer(priority);
	}

	bool EngineController::removeTimerID(uint64_t id) {
		return _coreController->removeTimerID(id);
	}

	uint64_t EngineController::getTimeLeft(uint64_t id) const {
		return _coreController->getTimeLeft(id);
	}

	void EngineController::stop() {
		boost::thread thrd(&EngineController::ShutDown, EngineController::GetSingletonPtr());
		thrd.detach();
	}

	uint64_t EngineController::getCurrentTime() const {
		return _coreController->getCurrentTime();
	}

	void EngineController::setCurrentTime(uint64_t time) {
		_coreController->setCurrentTime(time);
	}

	uint32_t EngineController::getNewUUID() const {
		boost::hash<boost::uuids::uuid> uuid_hasher;
		return uuid_hasher(boost::uuids::random_generator()());
	}

} /* namespace api */
} /* namespace i6engine */
