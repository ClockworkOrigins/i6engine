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

#include "i6engine/api/Application.h"
#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/ScriptingFacade.h"
#include "i6engine/api/manager/IDManager.h"
#include "i6engine/api/manager/LanguageManager.h"
#include "i6engine/api/manager/TextManager.h"
#include "i6engine/api/manager/WaynetManager.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
namespace api {

	void registerSubSystem(const std::string & name, i6e::core::ModuleController * module, uint32_t frameTime) {
		i6e::api::EngineController::GetSingleton().registerSubSystem(name, module, frameTime);
	}

	void registerSubSystem(const std::string & name, i6e::core::ModuleController * module, const std::set<i6e::core::Subsystem> & waitingFor) {
		i6e::api::EngineController::GetSingleton().registerSubSystem(name, module, waitingFor);
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

	void setType(i6e::api::GameType type) {
		i6e::api::EngineController::GetSingleton().setType(type);
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

	i6e::api::GameType getType() {
		return i6e::api::EngineController::GetSingleton().getType();
	}

	struct ApplicationWrapper : public i6e::api::Application, public boost::python::wrapper<i6e::api::Application> {
		ApplicationWrapper() : Application(), boost::python::wrapper<i6e::api::Application>() {
		}

		ApplicationWrapper(const i6e::api::Application &) : Application(), boost::python::wrapper<i6e::api::Application>() {
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

		virtual void disconnectCallback(const i6e::core::IPKey & key) {
			if (boost::python::override dc = this->get_override("disconnectCallback")) {
				boost::python::call<void>(dc.ptr(), key);
				return;
			}
			Application::disconnectCallback(key);
		}

		void default_disconnectCallback(const i6e::core::IPKey & key) {
			this->Application::disconnectCallback(key);
		}

		virtual void connectionFailed(const i6e::core::IPKey & s) {
			if (boost::python::override cf = this->get_override("connectionFailed")) {
				boost::python::call<void>(cf.ptr(), s);
				return;
			}
			Application::connectionFailed(s);
		}

		void default_connectionFailed(const i6e::core::IPKey & s) {
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

		virtual void connected(const i6e::core::IPKey & key) {
			if (boost::python::override c = this->get_override("connected")) {
				boost::python::call<void>(c.ptr(), key);
				return;
			}
			Application::connected(key);
		}

		void default_connected(const i6e::core::IPKey & key) {
			this->Application::connected(key);
		}
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
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingAPIPython) {
	using namespace boost::python;
	enum_<i6e::api::messages::EngineMessageTypes>("EngineMessageTypes")
		.value("AdministrationMessageType", i6e::api::messages::EngineMessageTypes::AdministrationMessageType)
		.value("ComponentMessageType", i6e::api::messages::EngineMessageTypes::ComponentMessageType)
		.value("GameMessageType", i6e::api::messages::EngineMessageTypes::GameMessageType)
		.value("GraphicsMessageType", i6e::api::messages::EngineMessageTypes::GraphicsMessageType)
		.value("GraphicsNodeMessageType", i6e::api::messages::EngineMessageTypes::GraphicsNodeMessageType)
		.value("GUIMessageType", i6e::api::messages::EngineMessageTypes::GUIMessageType)
		.value("InputMessageType", i6e::api::messages::EngineMessageTypes::InputMessageType)
		.value("ObjectManagerMessageType", i6e::api::messages::EngineMessageTypes::ObjectManagerMessageType)
		.value("NetworkMessageType", i6e::api::messages::EngineMessageTypes::NetworkMessageType)
		.value("ObjectMessageType", i6e::api::messages::EngineMessageTypes::ObjectMessageType)
		.value("PhysicsMessageType", i6e::api::messages::EngineMessageTypes::PhysicsMessageType)
		.value("PhysicsNodeMessageType", i6e::api::messages::EngineMessageTypes::PhysicsNodeMessageType)
		.value("ScriptingMessageType", i6e::api::messages::EngineMessageTypes::ScriptingMessageType)
		.value("PositionMessageType", i6e::api::messages::EngineMessageTypes::PositionMessageType)
		.value("AudioMessageType", i6e::api::messages::EngineMessageTypes::AudioMessageType)
		.value("AudioNodeMessageType", i6e::api::messages::EngineMessageTypes::AudioNodeMessageType)
		.value("ChatMessageType", i6e::api::messages::EngineMessageTypes::ChatMessageType)
		.value("MessageTypeCount", i6e::api::messages::EngineMessageTypes::MessageTypeCount)
		.export_values();

	enum_<i6e::core::JobPriorities>("JobPriorities")
		.value("Prio_High", i6e::core::JobPriorities::Prio_High)
		.value("Prio_Medium", i6e::core::JobPriorities::Prio_Medium)
		.value("Prio_Low", i6e::core::JobPriorities::Prio_Low)
		.export_values();

	def("registerSubSystem", (void(*)(const std::string &, i6e::core::ModuleController *, uint32_t)) &i6e::python::api::getCurrentTime);
	def("registerSubSystem", (void(*)(const std::string &, i6e::core::ModuleController *, const std::set<i6e::core::Subsystem> &)) &i6e::python::api::getCurrentTime);
	def("getIDManager", &i6e::python::api::getIDManager, return_value_policy<reference_existing_object>());
	def("getLanguageManager", &i6e::python::api::getLanguageManager, return_value_policy<reference_existing_object>());
	def("getTextManager", &i6e::python::api::getTextManager, return_value_policy<reference_existing_object>());
	def("getWaynetManager", &i6e::python::api::getWaynetManager, return_value_policy<reference_existing_object>());
	def("registerApplication", &i6e::python::api::registerApplication);
	def("start", &i6e::python::api::start);
	def("setType", &i6e::python::api::setType);
	def("setDebugdrawer", &i6e::python::api::setDebugdrawer);
	def("getDebugdrawer", &i6e::python::api::getDebugdrawer);
	def("getAppl", &i6e::python::api::getAppl, return_internal_reference<>());
	def("registerDefault", &i6e::python::api::registerDefault);
	def("stop", &i6e::python::api::stop);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, i6e::core::JobPriorities)) &i6e::python::api::registerTimer);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, i6e::core::JobPriorities)) &i6e::python::api::registerTimer);
	def("removeTimer", &i6e::python::api::removeTimer);
	def("removeTimerID", &i6e::python::api::removeTimerID);
	def("getTimeLeft", &i6e::python::api::getTimeLeft);
	def("getCurrentTime", &i6e::python::api::getCurrentTime);
	def("setCurrentTime", &i6e::python::api::setCurrentTime);
	def("getUUID", &i6e::python::api::getUUID);
	def("getNewUUID", &i6e::python::api::getNewUUID);
	def("getType", &i6e::python::api::getType);

	class_<i6e::api::Application, i6e::python::api::ApplicationWrapper, boost::noncopyable>("Application", no_init)
		.def("Initialize", pure_virtual(&i6e::python::api::ApplicationWrapper::Initialize))
		.def("AfterInitialize", pure_virtual(&i6e::python::api::ApplicationWrapper::AfterInitialize))
		.def("Tick", pure_virtual(&i6e::python::api::ApplicationWrapper::Tick))
		.def("ShutdownRequest", pure_virtual(&i6e::python::api::ApplicationWrapper::ShutdownRequest))
		.def("Finalize", pure_virtual(&i6e::python::api::ApplicationWrapper::Finalize))
		.def("disconnectCallback", &i6e::api::Application::disconnectCallback, &i6e::python::api::ApplicationWrapper::default_disconnectCallback)
		.def("connectionFailed", &i6e::api::Application::connectionFailed, &i6e::python::api::ApplicationWrapper::default_connectionFailed)
		.def("bindFailure", &i6e::api::Application::bindFailure, &i6e::python::api::ApplicationWrapper::default_bindFailure)
		.def("connected", &i6e::api::Application::connected, &i6e::python::api::ApplicationWrapper::default_connected)
		.def("getName", &i6e::api::Application::getName)
		.def("setName", &i6e::api::Application::setName);

	class_<i6e::api::IDManager, boost::noncopyable>("IDManager", no_init)
		.def("setBounds", &i6e::api::IDManager::setBounds)
		.def("getID", (int64_t(i6e::api::IDManager::*)()) &i6e::api::IDManager::getID)
		.def("getID", (int64_t(i6e::api::IDManager::*)(uint32_t)) &i6e::api::IDManager::getID);

	class_<i6e::api::LanguageManager, boost::noncopyable>("LanguageManager", no_init)
		.def("addCallback", &i6e::python::api::addLanguageScriptCallback)
		.def("addCallback", &i6e::python::api::addLanguageFunctionCallback)
		.def("setLanguage", &i6e::api::LanguageManager::setLanguage);

	class_<i6e::api::TextManager>("TextManager", no_init)
		.def("getText", &i6e::api::TextManager::getText);

	class_<i6e::api::WaynetManager, boost::noncopyable>("WaynetManager", no_init)
		.def("createWaynet", &i6e::api::WaynetManager::createWaynet)
		.def("getShortestPath", (std::vector<Vec3>(i6e::api::WaynetManager::*)(const Vec3 &, const std::string &)) &i6e::api::WaynetManager::getShortestPath)
		.def("showWaynet", &i6e::api::WaynetManager::showWaynet)
		.def("getNearestWaypoint", &i6e::api::WaynetManager::getNearestWaypoint);
}
