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

namespace i6engine {
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

	void registerSubSystem(const std::string & name, i6engine::core::ModuleController * module, uint32_t frameTime) {
		i6engine::api::EngineController::GetSingleton().registerSubSystem(name, module, frameTime);
	}

	void registerSubSystem(const std::string & name, i6engine::core::ModuleController * module, const std::set<core::Subsystem::Type> & waitingFor) {
		std::set<i6engine::core::Subsystem> waitingFor2;

		for (core::Subsystem::Type s : waitingFor) {
			waitingFor2.insert(i6engine::core::Subsystem(int(s)));
		}

		i6engine::api::EngineController::GetSingleton().registerSubSystem(name, module, waitingFor2);
	}

	i6engine::api::IDManager * getIDManager() {
		return i6engine::api::EngineController::GetSingleton().getIDManager();
	}

	i6engine::api::LanguageManager * getLanguageManager() {
		return i6engine::api::EngineController::GetSingleton().getLanguageManager();
	}

	i6engine::api::TextManager * getTextManager() {
		return i6engine::api::EngineController::GetSingleton().getTextManager();
	}

	i6engine::api::WaynetManager * getWaynetManager() {
		return i6engine::api::EngineController::GetSingleton().getWaynetManager();
	}

	void registerApplication(i6engine::api::Application & app) {
		i6engine::api::EngineController::GetSingleton().registerApplication(app);
	}

	void start() {
		i6engine::api::EngineController::GetSingleton().start();
	}

	void setType(i6engine::lua::api::GameType::Type type) {
		i6engine::api::EngineController::GetSingleton().setType(i6engine::api::GameType(type));
	}

	void setDebugdrawer(uint8_t dd) {
		i6engine::api::EngineController::GetSingleton().setDebugdrawer(dd);
	}

	uint8_t getDebugdrawer() {
		return i6engine::api::EngineController::GetSingleton().getDebugdrawer();
	}

	i6engine::api::Application * getAppl() {
		return i6engine::api::EngineController::GetSingleton().getAppl();
	}

	void registerDefault(bool ds) {
		i6engine::api::EngineController::GetSingleton().registerDefault(ds);
	}

	void stop() {
		i6engine::api::EngineController::GetSingleton().stop();
	}

	uint64_t registerTimer(uint64_t time, const std::string & file, const std::string & func, bool looping, i6engine::core::JobPriorities priority) {
		return i6engine::api::EngineController::GetSingleton().registerTimer(time, [file, func]() {
			auto ret = i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<bool>(file, func);
			return ret->get();
		}, looping, priority);
	}

	uint64_t registerTimer(uint64_t time, const std::string & func, bool looping, i6engine::core::JobPriorities priority) {
		return i6engine::api::EngineController::GetSingleton().registerTimer(time, [func]() {
			auto ret = i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(func);
			return ret->get();
		}, looping, priority);
	}

	void removeTimer(i6engine::core::JobPriorities priority) {
		i6engine::api::EngineController::GetSingleton().removeTimer(priority);
	}

	bool removeTimerID(uint64_t id) {
		return i6engine::api::EngineController::GetSingleton().removeTimerID(id);
	}

	uint64_t getTimeLeft(uint64_t id) {
		return i6engine::api::EngineController::GetSingleton().getTimeLeft(id);
	}

	uint64_t getCurrentTime() {
		return i6engine::api::EngineController::GetSingleton().getCurrentTime();
	}

	void setCurrentTime(uint64_t time) {
		i6engine::api::EngineController::GetSingleton().setCurrentTime(time);
	}

	uint32_t getUUID() {
		return i6engine::api::EngineController::GetSingleton().getUUID();
	}

	uint32_t getNewUUID() {
		return i6engine::api::EngineController::GetSingleton().getNewUUID();
	}

	i6engine::lua::api::GameType::Type getType() {
		return i6engine::lua::api::GameType::Type(int(i6engine::api::EngineController::GetSingleton().getType()));
	}

	struct ApplicationWrapper : i6engine::api::Application, public luabind::wrap_base {
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

		virtual void disconnectCallback(const i6engine::core::IPKey & key) {
			luabind::call_member<void>(this, "disconnectCallback", key);
		}

		static void default_disconnectCallback(i6engine::api::Application * ptr, const i6engine::core::IPKey & key) {
			ptr->Application::disconnectCallback(key);
		}

		virtual void connectionFailed(const i6engine::core::IPKey & s) {
			luabind::call_member<void>(this, "connectionFailed", s);
		}

		static void default_connectionFailed(i6engine::api::Application * ptr, const i6engine::core::IPKey & s) {
			ptr->Application::connectionFailed(s);
		}

		virtual void bindFailure() {
			luabind::call_member<void>(this, "bindFailure");
		}

		static void default_bindFailure(i6engine::api::Application * ptr) {
			ptr->Application::bindFailure();
		}

		virtual void connected(const i6engine::core::IPKey & key) {
			luabind::call_member<void>(this, "connected", key);
		}

		static void default_connected(i6engine::api::Application * ptr, const i6engine::core::IPKey & key) {
			ptr->Application::connected(key);
		}
	};

	class EngineMessageTypes {
	};

	void addLanguageScriptCallback(i6engine::api::LanguageManager * lm, const std::string & file, const std::string & func) {
		lm->addCallback([file, func](std::string language) {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<void>(file, func, language);
		});
	}

	void addLanguageFunctionCallback(i6engine::api::LanguageManager * lm, const std::string & func) {
		lm->addCallback([func](std::string language) {
			i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<void>(func, language);
		});
	}

} /* namespace api */
} /* namespace lua */
} /* namespace i6engine */

using namespace luabind;

scope registerAPI() {
	return
		class_<i6engine::lua::api::GameType>("EngineMessageTypes")
			.def(constructor<>())
			.enum_("EngineMessageTypes")
			[
				value("AdministrationMessageType", int(i6engine::api::messages::EngineMessageTypes::AdministrationMessageType)),
				value("ComponentMessageType", int(i6engine::api::messages::EngineMessageTypes::ComponentMessageType)),
				value("GameMessageType", int(i6engine::api::messages::EngineMessageTypes::GameMessageType)),
				value("GraphicsMessageType", int(i6engine::api::messages::EngineMessageTypes::GraphicsMessageType)),
				value("GraphicsNodeMessageType", int(i6engine::api::messages::EngineMessageTypes::GraphicsNodeMessageType)),
				value("GUIMessageType", int(i6engine::api::messages::EngineMessageTypes::GUIMessageType)),
				value("InputMessageType", int(i6engine::api::messages::EngineMessageTypes::InputMessageType)),
				value("ObjectManagerMessageType", int(i6engine::api::messages::EngineMessageTypes::ObjectManagerMessageType)),
				value("NetworkMessageType", int(i6engine::api::messages::EngineMessageTypes::NetworkMessageType)),
				value("ObjectMessageType", int(i6engine::api::messages::EngineMessageTypes::ObjectMessageType)),
				value("PhysicsMessageType", int(i6engine::api::messages::EngineMessageTypes::PhysicsMessageType)),
				value("PhysicsNodeMessageType", int(i6engine::api::messages::EngineMessageTypes::PhysicsNodeMessageType)),
				value("ScriptingMessageType", int(i6engine::api::messages::EngineMessageTypes::ScriptingMessageType)),
				value("PositionMessageType", int(i6engine::api::messages::EngineMessageTypes::PositionMessageType)),
				value("AudioMessageType", int(i6engine::api::messages::EngineMessageTypes::AudioMessageType)),
				value("AudioNodeMessageType", int(i6engine::api::messages::EngineMessageTypes::AudioNodeMessageType)),
				value("ChatMessageType", int(i6engine::api::messages::EngineMessageTypes::ChatMessageType)),
				value("MessageTypeCount", int(i6engine::api::messages::EngineMessageTypes::MessageTypeCount))
			],

		class_<i6engine::core::JobPriorities>("JobPriorities")
			.def(constructor<>())
			.enum_("JobPriorities")
			[
				value("Prio_High", i6engine::core::JobPriorities::Prio_High),
				value("Prio_Medium", i6engine::core::JobPriorities::Prio_Medium),
				value("Prio_Low", i6engine::core::JobPriorities::Prio_Low)
			],

	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, uint32_t)) &i6engine::lua::api::registerSubSystem),
	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, const std::set<i6engine::lua::core::Subsystem::Type> &)) &i6engine::lua::api::registerSubSystem),
	def("getIDManager", &i6engine::lua::api::getIDManager),
	def("getLanguageManager", &i6engine::lua::api::getLanguageManager),
	def("getTextManager", &i6engine::lua::api::getTextManager),
	def("getWaynetManager", &i6engine::lua::api::getWaynetManager),
	def("registerApplication", &i6engine::lua::api::registerApplication),
	def("start", &i6engine::lua::api::start),
	def("setType", &i6engine::lua::api::setType),
	def("setDebugdrawer", &i6engine::lua::api::setDebugdrawer),
	def("getDebugdrawer", &i6engine::lua::api::getDebugdrawer),
	def("getAppl", &i6engine::lua::api::getAppl),
	def("registerDefault", &i6engine::lua::api::registerDefault),
	def("stop", &i6engine::lua::api::stop),
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, i6engine::core::JobPriorities)) &i6engine::lua::api::registerTimer),
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, i6engine::core::JobPriorities)) &i6engine::lua::api::registerTimer),
	def("removeTimer", &i6engine::lua::api::removeTimer),
	def("removeTimerID", &i6engine::lua::api::removeTimerID),
	def("getTimeLeft", &i6engine::lua::api::getTimeLeft),
	def("getCurrentTime", &i6engine::lua::api::getCurrentTime),
	def("setCurrentTime", &i6engine::lua::api::setCurrentTime),
	def("getUUID", &i6engine::lua::api::getUUID),
	def("getNewUUID", &i6engine::lua::api::getNewUUID),
	def("getType", &i6engine::lua::api::getType),

	class_<i6engine::api::Application, i6engine::lua::api::ApplicationWrapper>("Application")
		.def(constructor<>())
		.def("Initialize", &i6engine::lua::api::ApplicationWrapper::Initialize)
		.def("AfterInitialize", &i6engine::lua::api::ApplicationWrapper::AfterInitialize)
		.def("Tick", &i6engine::lua::api::ApplicationWrapper::Tick)
		.def("ShutdownRequest", &i6engine::lua::api::ApplicationWrapper::ShutdownRequest)
		.def("Finalize", &i6engine::lua::api::ApplicationWrapper::Finalize)
		.def("disconnectCallback", &i6engine::api::Application::disconnectCallback, &i6engine::lua::api::ApplicationWrapper::default_disconnectCallback)
		.def("connectionFailed", &i6engine::api::Application::connectionFailed, &i6engine::lua::api::ApplicationWrapper::default_connectionFailed)
		.def("bindFailure", &i6engine::api::Application::bindFailure, &i6engine::lua::api::ApplicationWrapper::default_bindFailure)
		.def("connected", &i6engine::api::Application::connected, &i6engine::lua::api::ApplicationWrapper::default_connected)
		.def("getName", &i6engine::api::Application::getName)
		.def("setName", &i6engine::api::Application::setName),

	class_<i6engine::api::IDManager>("IDManager")
		.def(constructor<>())
		.def("setBounds", &i6engine::api::IDManager::setBounds)
		.def("getID", (int64_t(i6engine::api::IDManager::*)()) &i6engine::api::IDManager::getID)
		.def("getID", (int64_t(i6engine::api::IDManager::*)(uint32_t)) &i6engine::api::IDManager::getID),

	class_<i6engine::api::LanguageManager>("LanguageManager")
		.def(constructor<>())
		.def("addCallback", &i6engine::lua::api::addLanguageScriptCallback)
		.def("addCallback", &i6engine::lua::api::addLanguageFunctionCallback)
		.def("setLanguage", &i6engine::api::LanguageManager::setLanguage),

	class_<i6engine::api::TextManager>("TextManager")
		.def(constructor<>())
		.def("getText", &i6engine::api::TextManager::getText),

	class_<i6engine::api::WaynetManager>("WaynetManager")
		.def(constructor<>())
		.def("createWaynet", &i6engine::api::WaynetManager::createWaynet)
		.def("getShortestPath", (std::vector<Vec3>(i6engine::api::WaynetManager::*)(const Vec3 &, const std::string &)) &i6engine::api::WaynetManager::getShortestPath)
		.def("showWaynet", &i6engine::api::WaynetManager::showWaynet)
		.def("getNearestWaypoint", &i6engine::api::WaynetManager::getNearestWaypoint)
		;
}
