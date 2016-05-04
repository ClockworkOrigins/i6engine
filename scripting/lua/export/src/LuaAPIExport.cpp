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

#include "LuaAPIExport.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/manager/WaynetManager.h"

namespace i6e {
namespace lua {
namespace core {

	class Subsystem {
	public:
		enum Type {
			Unknown,
			Application,
			Audio,
			Graphic,
			GUI,
			Input,
			Network,
			Object,
			Physic,
			Scripting
		};
	};

} /* namespace core */
namespace api {

	class GameType {
	public:
		enum Type {
			SINGLEPLAYER,
			CLIENT,
			SERVER
		};
	};

	void registerSubSystem(const std::string & name, i6e::core::ModuleController * module, uint32_t frameTime) {
		i6e::api::EngineController::GetSingleton().registerSubSystem(name, module, frameTime);
	}

	void registerSubSystem(const std::string & name, i6e::core::ModuleController * module, const std::set<core::Subsystem::Type> & waitingFor) {
		std::set<i6e::core::Subsystem> waitingFor2;

		for (core::Subsystem::Type s : waitingFor) {
			waitingFor2.insert(i6e::core::Subsystem(int(s)));
		}

		i6e::api::EngineController::GetSingleton().registerSubSystem(name, module, waitingFor2);
	}

	i6e::api::IDManager * getIDManager() {
		return i6e::api::EngineController::GetSingleton().getIDManager();
	}

	i6e::api::LanguageManager * getLanguageManager() {
		return i6e::api::EngineController::GetSingleton().getLanguageManager();
	}

	i6e::api::TextManager * getTextManager() {
		return i6e::api::EngineController::GetSingleton().getTextManager();
	}

	i6e::api::WaynetManager * getWaynetManager() {
		return i6e::api::EngineController::GetSingleton().getWaynetManager();
	}

	void registerApplication(i6e::api::Application & app) {
		i6e::api::EngineController::GetSingleton().registerApplication(app);
	}

	void start() {
		i6e::api::EngineController::GetSingleton().start();
	}

	void setType(i6e::lua::api::GameType::Type type) {
		i6e::api::EngineController::GetSingleton().setType(i6e::api::GameType(type));
	}

	void setDebugdrawer(uint8_t dd) {
		i6e::api::EngineController::GetSingleton().setDebugdrawer(dd);
	}

	uint8_t getDebugdrawer() {
		return i6e::api::EngineController::GetSingleton().getDebugdrawer();
	}

	i6e::api::Application * getAppl() {
		return i6e::api::EngineController::GetSingleton().getAppl();
	}

	void registerDefault(bool ds) {
		i6e::api::EngineController::GetSingleton().registerDefault(ds);
	}

	void stop() {
		i6e::api::EngineController::GetSingleton().stop();
	}

	uint64_t registerTimer(uint64_t time, const std::string & file, const std::string & func, bool looping, i6e::core::JobPriorities priority) {
		return i6e::api::EngineController::GetSingleton().registerTimer(time, [file, func]() {
			auto ret = i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<bool>(file, func);
			return ret->get();
		}, looping, priority);
	}

	uint64_t registerTimer(uint64_t time, const std::string & func, bool looping, i6e::core::JobPriorities priority) {
		return i6e::api::EngineController::GetSingleton().registerTimer(time, [func]() {
			auto ret = i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(func);
			return ret->get();
		}, looping, priority);
	}

	void removeTimer(i6e::core::JobPriorities priority) {
		i6e::api::EngineController::GetSingleton().removeTimer(priority);
	}

	bool removeTimerID(uint64_t id) {
		return i6e::api::EngineController::GetSingleton().removeTimerID(id);
	}

	uint64_t getTimeLeft(uint64_t id) {
		return i6e::api::EngineController::GetSingleton().getTimeLeft(id);
	}

	uint64_t getCurrentTime() {
		return i6e::api::EngineController::GetSingleton().getCurrentTime();
	}

	void setCurrentTime(uint64_t time) {
		i6e::api::EngineController::GetSingleton().setCurrentTime(time);
	}

	uint64_t getUUID() {
		return i6e::api::EngineController::GetSingleton().getUUID();
	}

	uint64_t getNewUUID() {
		return i6e::api::EngineController::GetSingleton().getNewUUID();
	}

	i6e::lua::api::GameType::Type getType() {
		return i6e::lua::api::GameType::Type(int(i6e::api::EngineController::GetSingleton().getType()));
	}

	struct ApplicationWrapper : i6e::api::Application, public luabind::wrap_base {
		ApplicationWrapper() : Application(), luabind::wrap_base() {
		}

		virtual void Initialize() {
			luabind::call_member<void>(this, "Initialize");
		}

		virtual void AfterInitialize() {
			luabind::call_member<void>(this, "AfterInitialize");
		}

		virtual void Tick() {
			luabind::call_member<void>(this, "Tick");
		}

		virtual bool ShutdownRequest() {
			return luabind::call_member<bool>(this, "ShutdownRequest");
		}

		virtual void Finalize() {
			luabind::call_member<void>(this, "Finalize");
		}

		virtual void disconnectCallback(const i6e::core::IPKey & key) {
			luabind::call_member<void>(this, "disconnectCallback", key);
		}

		static void default_disconnectCallback(i6e::api::Application * ptr, const i6e::core::IPKey & key) {
			ptr->Application::disconnectCallback(key);
		}

		virtual void connectionFailed(const i6e::core::IPKey & s) {
			luabind::call_member<void>(this, "connectionFailed", s);
		}

		static void default_connectionFailed(i6e::api::Application * ptr, const i6e::core::IPKey & s) {
			ptr->Application::connectionFailed(s);
		}

		virtual void bindFailure() {
			luabind::call_member<void>(this, "bindFailure");
		}

		static void default_bindFailure(i6e::api::Application * ptr) {
			ptr->Application::bindFailure();
		}

		virtual void connected(const i6e::core::IPKey & key) {
			luabind::call_member<void>(this, "connected", key);
		}

		static void default_connected(i6e::api::Application * ptr, const i6e::core::IPKey & key) {
			ptr->Application::connected(key);
		}
	};

	class EngineMessageTypes {
	};

	void addLanguageScriptCallback(i6e::api::LanguageManager * lm, const std::string & file, const std::string & func) {
		lm->addCallback([file, func](std::string language) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(file, func, language);
		});
	}

	void addLanguageFunctionCallback(i6e::api::LanguageManager * lm, const std::string & func) {
		lm->addCallback([func](std::string language) {
			i6e::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, language);
		});
	}

} /* namespace api */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerAPI() {
	return
		class_<i6e::lua::api::GameType>("EngineMessageTypes")
			.def(constructor<>())
			.enum_("EngineMessageTypes")
			[
				value("AdministrationMessageType", int(i6e::api::messages::EngineMessageTypes::AdministrationMessageType)),
				value("ComponentMessageType", int(i6e::api::messages::EngineMessageTypes::ComponentMessageType)),
				value("GameMessageType", int(i6e::api::messages::EngineMessageTypes::GameMessageType)),
				value("GraphicsMessageType", int(i6e::api::messages::EngineMessageTypes::GraphicsMessageType)),
				value("GraphicsNodeMessageType", int(i6e::api::messages::EngineMessageTypes::GraphicsNodeMessageType)),
				value("GUIMessageType", int(i6e::api::messages::EngineMessageTypes::GUIMessageType)),
				value("InputMessageType", int(i6e::api::messages::EngineMessageTypes::InputMessageType)),
				value("ObjectManagerMessageType", int(i6e::api::messages::EngineMessageTypes::ObjectManagerMessageType)),
				value("NetworkMessageType", int(i6e::api::messages::EngineMessageTypes::NetworkMessageType)),
				value("ObjectMessageType", int(i6e::api::messages::EngineMessageTypes::ObjectMessageType)),
				value("PhysicsMessageType", int(i6e::api::messages::EngineMessageTypes::PhysicsMessageType)),
				value("PhysicsNodeMessageType", int(i6e::api::messages::EngineMessageTypes::PhysicsNodeMessageType)),
				value("ScriptingMessageType", int(i6e::api::messages::EngineMessageTypes::ScriptingMessageType)),
				value("PositionMessageType", int(i6e::api::messages::EngineMessageTypes::PositionMessageType)),
				value("AudioMessageType", int(i6e::api::messages::EngineMessageTypes::AudioMessageType)),
				value("AudioNodeMessageType", int(i6e::api::messages::EngineMessageTypes::AudioNodeMessageType)),
				value("ChatMessageType", int(i6e::api::messages::EngineMessageTypes::ChatMessageType)),
				value("MessageTypeCount", int(i6e::api::messages::EngineMessageTypes::MessageTypeCount))
			],

		class_<i6e::core::JobPriorities>("JobPriorities")
			.def(constructor<>())
			.enum_("JobPriorities")
			[
				value("Prio_High", i6e::core::JobPriorities::Prio_High),
				value("Prio_Medium", i6e::core::JobPriorities::Prio_Medium),
				value("Prio_Low", i6e::core::JobPriorities::Prio_Low)
			],

	def("registerSubSystem", (void(*)(const std::string &, i6e::core::ModuleController *, uint32_t)) &i6e::lua::api::registerSubSystem),
	def("registerSubSystem", (void(*)(const std::string &, i6e::core::ModuleController *, const std::set<i6e::lua::core::Subsystem::Type> &)) &i6e::lua::api::registerSubSystem),
	def("getIDManager", &i6e::lua::api::getIDManager),
	def("getLanguageManager", &i6e::lua::api::getLanguageManager),
	def("getTextManager", &i6e::lua::api::getTextManager),
	def("getWaynetManager", &i6e::lua::api::getWaynetManager),
	def("registerApplication", &i6e::lua::api::registerApplication),
	def("start", &i6e::lua::api::start),
	def("setType", &i6e::lua::api::setType),
	def("setDebugdrawer", &i6e::lua::api::setDebugdrawer),
	def("getDebugdrawer", &i6e::lua::api::getDebugdrawer),
	def("getAppl", &i6e::lua::api::getAppl),
	def("registerDefault", &i6e::lua::api::registerDefault),
	def("stop", &i6e::lua::api::stop),
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, i6e::core::JobPriorities)) &i6e::lua::api::registerTimer),
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, i6e::core::JobPriorities)) &i6e::lua::api::registerTimer),
	def("removeTimer", &i6e::lua::api::removeTimer),
	def("removeTimerID", &i6e::lua::api::removeTimerID),
	def("getTimeLeft", &i6e::lua::api::getTimeLeft),
	def("getCurrentTime", &i6e::lua::api::getCurrentTime),
	def("setCurrentTime", &i6e::lua::api::setCurrentTime),
	def("getUUID", &i6e::lua::api::getUUID),
	def("getNewUUID", &i6e::lua::api::getNewUUID),
	def("getType", &i6e::lua::api::getType),

	class_<i6e::api::Application, i6e::lua::api::ApplicationWrapper>("Application")
		.def(constructor<>())
		.def("Initialize", &i6e::lua::api::ApplicationWrapper::Initialize)
		.def("AfterInitialize", &i6e::lua::api::ApplicationWrapper::AfterInitialize)
		.def("Tick", &i6e::lua::api::ApplicationWrapper::Tick)
		.def("ShutdownRequest", &i6e::lua::api::ApplicationWrapper::ShutdownRequest)
		.def("Finalize", &i6e::lua::api::ApplicationWrapper::Finalize)
		.def("disconnectCallback", &i6e::api::Application::disconnectCallback, &i6e::lua::api::ApplicationWrapper::default_disconnectCallback)
		.def("connectionFailed", &i6e::api::Application::connectionFailed, &i6e::lua::api::ApplicationWrapper::default_connectionFailed)
		.def("bindFailure", &i6e::api::Application::bindFailure, &i6e::lua::api::ApplicationWrapper::default_bindFailure)
		.def("connected", &i6e::api::Application::connected, &i6e::lua::api::ApplicationWrapper::default_connected)
		.def("getName", &i6e::api::Application::getName)
		.def("setName", &i6e::api::Application::setName),

	class_<i6e::api::IDManager>("IDManager")
		.def(constructor<>())
		.def("setBounds", &i6e::api::IDManager::setBounds)
		.def("getID", (int64_t(i6e::api::IDManager::*)()) &i6e::api::IDManager::getID)
		.def("getID", (int64_t(i6e::api::IDManager::*)(uint32_t)) &i6e::api::IDManager::getID),

	class_<i6e::api::LanguageManager>("LanguageManager")
		.def(constructor<>())
		.def("addCallback", &i6e::lua::api::addLanguageScriptCallback)
		.def("addCallback", &i6e::lua::api::addLanguageFunctionCallback)
		.def("setLanguage", &i6e::api::LanguageManager::setLanguage),

	class_<i6e::api::TextManager>("TextManager")
		.def(constructor<>())
		.def("getText", &i6e::api::TextManager::getText),

	class_<i6e::api::WaynetManager>("WaynetManager")
		.def(constructor<>())
		.def("createWaynet", &i6e::api::WaynetManager::createWaynet)
		.def("getShortestPath", (std::vector<Vec3>(i6e::api::WaynetManager::*)(const Vec3 &, const std::string &)) &i6e::api::WaynetManager::getShortestPath)
		.def("showWaynet", &i6e::api::WaynetManager::showWaynet)
		.def("getNearestWaypoint", &i6e::api::WaynetManager::getNearestWaypoint)
		;
}
