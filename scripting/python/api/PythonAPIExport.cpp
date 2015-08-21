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

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/manager/WaynetManager.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace api {

	void registerSubSystem(const std::string & name, i6engine::core::ModuleController * module, uint32_t frameTime) {
		i6engine::api::EngineController::GetSingleton().registerSubSystem(name, module, frameTime);
	}

	void registerSubSystem(const std::string & name, i6engine::core::ModuleController * module, const std::set<i6engine::core::Subsystem> & waitingFor) {
		i6engine::api::EngineController::GetSingleton().registerSubSystem(name, module, waitingFor);
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

	void setType(i6engine::api::GameType type) {
		i6engine::api::EngineController::GetSingleton().setType(type);
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

	i6engine::api::GameType getType() {
		return i6engine::api::EngineController::GetSingleton().getType();
	}

	struct ApplicationWrapper : public i6engine::api::Application, public boost::python::wrapper<i6engine::api::Application> {
		ApplicationWrapper() : Application(), boost::python::wrapper<i6engine::api::Application>() {
		}

		ApplicationWrapper(const i6engine::api::Application & arg) : Application(), boost::python::wrapper<i6engine::api::Application>() {
		}

		virtual void Initialize() {
			boost::python::call<void>(this->get_override("Initialize").ptr());
		}

		virtual void AfterInitialize() {
			boost::python::call<void>(this->get_override("AfterInitialize").ptr());
		}

		virtual void Tick() {
			boost::python::call<void>(this->get_override("Tick").ptr());
		}

		virtual bool ShutdownRequest() {
			return boost::python::call<bool>(this->get_override("ShutdownRequest").ptr());
		}

		virtual void Finalize() {
			boost::python::call<void>(this->get_override("Finalize").ptr());
		}

		virtual void disconnectCallback(const i6engine::core::IPKey & key) {
			if (boost::python::override dc = this->get_override("disconnectCallback")) {
				boost::python::call<void>(dc.ptr(), key);
				return;
			}
			Application::disconnectCallback(key);
		}

		void default_disconnectCallback(const i6engine::core::IPKey & key) {
			this->Application::disconnectCallback(key);
		}

		virtual void connectionFailed(const i6engine::core::IPKey & s) {
			if (boost::python::override cf = this->get_override("connectionFailed")) {
				boost::python::call<void>(cf.ptr(), s);
				return;
			}
			Application::connectionFailed(s);
		}

		void default_connectionFailed(const i6engine::core::IPKey & s) {
			this->Application::connectionFailed(s);
		}

		virtual void bindFailure() {
			if (boost::python::override bf = this->get_override("bindFailure")) {
				boost::python::call<void>(bf.ptr());
				return;
			}
			Application::bindFailure();
		}

		void default_bindFailure() {
			this->Application::bindFailure();
		}

		virtual void connected(const i6engine::core::IPKey & key) {
			if (boost::python::override c = this->get_override("connected")) {
				boost::python::call<void>(c.ptr(), key);
				return;
			}
			Application::connected(key);
		}

		void default_connected(const i6engine::core::IPKey & key) {
			this->Application::connected(key);
		}
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
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingAPIPython) {
	using namespace boost::python;
	enum_<i6engine::api::messages::EngineMessageTypes>("EngineMessageTypes")
		.value("AdministrationMessageType", i6engine::api::messages::EngineMessageTypes::AdministrationMessageType)
		.value("ComponentMessageType", i6engine::api::messages::EngineMessageTypes::ComponentMessageType)
		.value("GameMessageType", i6engine::api::messages::EngineMessageTypes::GameMessageType)
		.value("GraphicsMessageType", i6engine::api::messages::EngineMessageTypes::GraphicsMessageType)
		.value("GraphicsNodeMessageType", i6engine::api::messages::EngineMessageTypes::GraphicsNodeMessageType)
		.value("GUIMessageType", i6engine::api::messages::EngineMessageTypes::GUIMessageType)
		.value("InputMessageType", i6engine::api::messages::EngineMessageTypes::InputMessageType)
		.value("ObjectManagerMessageType", i6engine::api::messages::EngineMessageTypes::ObjectManagerMessageType)
		.value("NetworkMessageType", i6engine::api::messages::EngineMessageTypes::NetworkMessageType)
		.value("ObjectMessageType", i6engine::api::messages::EngineMessageTypes::ObjectMessageType)
		.value("PhysicsMessageType", i6engine::api::messages::EngineMessageTypes::PhysicsMessageType)
		.value("PhysicsNodeMessageType", i6engine::api::messages::EngineMessageTypes::PhysicsNodeMessageType)
		.value("ScriptingMessageType", i6engine::api::messages::EngineMessageTypes::ScriptingMessageType)
		.value("PositionMessageType", i6engine::api::messages::EngineMessageTypes::PositionMessageType)
		.value("AudioMessageType", i6engine::api::messages::EngineMessageTypes::AudioMessageType)
		.value("AudioNodeMessageType", i6engine::api::messages::EngineMessageTypes::AudioNodeMessageType)
		.value("ChatMessageType", i6engine::api::messages::EngineMessageTypes::ChatMessageType)
		.value("MessageTypeCount", i6engine::api::messages::EngineMessageTypes::MessageTypeCount)
		.export_values();

	enum_<i6engine::core::JobPriorities>("JobPriorities")
		.value("Prio_High", i6engine::core::JobPriorities::Prio_High)
		.value("Prio_Medium", i6engine::core::JobPriorities::Prio_Medium)
		.value("Prio_Low", i6engine::core::JobPriorities::Prio_Low)
		.export_values();

	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, uint32_t)) &i6engine::python::api::getCurrentTime);
	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, const std::set<i6engine::core::Subsystem> &)) &i6engine::python::api::getCurrentTime);
	def("getIDManager", &i6engine::python::api::getIDManager, return_value_policy<reference_existing_object>());
	def("getLanguageManager", &i6engine::python::api::getLanguageManager, return_value_policy<reference_existing_object>());
	def("getTextManager", &i6engine::python::api::getTextManager, return_value_policy<reference_existing_object>());
	def("getWaynetManager", &i6engine::python::api::getWaynetManager, return_value_policy<reference_existing_object>());
	def("registerApplication", &i6engine::python::api::registerApplication);
	def("start", &i6engine::python::api::start);
	def("setType", &i6engine::python::api::setType);
	def("setDebugdrawer", &i6engine::python::api::setDebugdrawer);
	def("getDebugdrawer", &i6engine::python::api::getDebugdrawer);
	def("getAppl", &i6engine::python::api::getAppl, return_internal_reference<>());
	def("registerDefault", &i6engine::python::api::registerDefault);
	def("stop", &i6engine::python::api::stop);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, i6engine::core::JobPriorities)) &i6engine::python::api::registerTimer);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, i6engine::core::JobPriorities)) &i6engine::python::api::registerTimer);
	def("removeTimer", &i6engine::python::api::removeTimer);
	def("removeTimerID", &i6engine::python::api::removeTimerID);
	def("getTimeLeft", &i6engine::python::api::getTimeLeft);
	def("getCurrentTime", &i6engine::python::api::getCurrentTime);
	def("setCurrentTime", &i6engine::python::api::setCurrentTime);
	def("getUUID", &i6engine::python::api::getUUID);
	def("getNewUUID", &i6engine::python::api::getNewUUID);
	def("getType", &i6engine::python::api::getType);

	class_<i6engine::api::Application, i6engine::python::api::ApplicationWrapper, boost::noncopyable>("Application", no_init)
		.def("Initialize", pure_virtual(&i6engine::python::api::ApplicationWrapper::Initialize))
		.def("AfterInitialize", pure_virtual(&i6engine::python::api::ApplicationWrapper::AfterInitialize))
		.def("Tick", pure_virtual(&i6engine::python::api::ApplicationWrapper::Tick))
		.def("ShutdownRequest", pure_virtual(&i6engine::python::api::ApplicationWrapper::ShutdownRequest))
		.def("Finalize", pure_virtual(&i6engine::python::api::ApplicationWrapper::Finalize))
		.def("disconnectCallback", &i6engine::api::Application::disconnectCallback, &i6engine::python::api::ApplicationWrapper::default_disconnectCallback)
		.def("connectionFailed", &i6engine::api::Application::connectionFailed, &i6engine::python::api::ApplicationWrapper::default_connectionFailed)
		.def("bindFailure", &i6engine::api::Application::bindFailure, &i6engine::python::api::ApplicationWrapper::default_bindFailure)
		.def("connected", &i6engine::api::Application::connected, &i6engine::python::api::ApplicationWrapper::default_connected)
		.def("getName", &i6engine::api::Application::getName)
		.def("setName", &i6engine::api::Application::setName);

	class_<i6engine::api::IDManager, boost::noncopyable>("IDManager", no_init)
		.def("setBounds", &i6engine::api::IDManager::setBounds)
		.def("getID", (int64_t(i6engine::api::IDManager::*)()) &i6engine::api::IDManager::getID)
		.def("getID", (int64_t(i6engine::api::IDManager::*)(uint32_t)) &i6engine::api::IDManager::getID);

	class_<i6engine::api::LanguageManager, boost::noncopyable>("LanguageManager", no_init)
		.def("addCallback", &i6engine::python::api::addLanguageScriptCallback)
		.def("addCallback", &i6engine::python::api::addLanguageFunctionCallback)
		.def("setLanguage", &i6engine::api::LanguageManager::setLanguage);

	class_<i6engine::api::TextManager>("TextManager", no_init)
		.def("getText", &i6engine::api::TextManager::getText);

	class_<i6engine::api::WaynetManager, boost::noncopyable>("WaynetManager", no_init)
		.def("createWaynet", &i6engine::api::WaynetManager::createWaynet)
		.def("getShortestPath", (std::vector<Vec3>(i6engine::api::WaynetManager::*)(const Vec3 &, const std::string &)) &i6engine::api::WaynetManager::getShortestPath)
		.def("showWaynet", &i6engine::api::WaynetManager::showWaynet)
		.def("getNearestWaypoint", &i6engine::api::WaynetManager::getNearestWaypoint);
}
