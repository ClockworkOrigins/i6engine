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
#include "i6engine/api/KeyCodes.h"
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

	uint64_t registerTimer(uint64_t time, const std::string & file, const std::string & func, bool looping, uint16_t priority) {
		return i6engine::api::EngineController::GetSingleton().registerTimer(time, [file, func]() {
			auto ret = i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callScript<bool>(file, func);
			return ret->get();
		}, looping, priority);
	}

	uint64_t registerTimer(uint64_t time, const std::string & func, bool looping, uint16_t priority) {
		return i6engine::api::EngineController::GetSingleton().registerTimer(time, [func]() {
			auto ret = i6engine::api::EngineController::GetSingleton().getScriptingFacade()->callFunction<bool>(func);
			return ret->get();
		}, looping, priority);
	}

	void removeTimer(uint16_t priority) {
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

	int32_t getUUID() {
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
			if (boost::python::override disconnectCallback = this->get_override("disconnectCallback")) {
				boost::python::call<void>(disconnectCallback.ptr(), key);
			}
			Application::disconnectCallback(key);
		}

		void default_disconnectCallback(const i6engine::core::IPKey & key) {
			this->Application::disconnectCallback(key);
		}

		virtual void connectionFailed(const i6engine::core::IPKey & s) {
			if (boost::python::override connectionFailed = this->get_override("connectionFailed")) {
				boost::python::call<void>(connectionFailed.ptr(), s);
			}
			Application::connectionFailed(s);
		}

		void default_connectionFailed(const i6engine::core::IPKey & s) {
			this->Application::connectionFailed(s);
		}

		virtual void bindFailure() {
			if (boost::python::override bindFailure = this->get_override("bindFailure")) {
				boost::python::call<void>(bindFailure.ptr());
			}
			Application::bindFailure();
		}

		void default_bindFailure() {
			this->Application::bindFailure();
		}

		virtual void connected(const i6engine::core::IPKey & key) {
			if (boost::python::override connected = this->get_override("connected")) {
				boost::python::call<void>(connected.ptr(), key);
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
		.value("MessageTypeCount", i6engine::api::messages::EngineMessageTypes::MessageTypeCount);

	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, uint32_t)) &i6engine::python::api::getCurrentTime);
	def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, const std::set<i6engine::core::Subsystem> &)) &i6engine::python::api::getCurrentTime);
	def("getIDManager", &i6engine::python::api::getIDManager);
	def("getLanguageManager", &i6engine::python::api::getLanguageManager);
	def("getTextManager", &i6engine::python::api::getTextManager);
	def("getWaynetManager", &i6engine::python::api::getWaynetManager);
	def("registerApplication", &i6engine::python::api::registerApplication);
	def("start", &i6engine::python::api::start);
	def("setType", &i6engine::python::api::setType);
	def("setDebugdrawer", &i6engine::python::api::setDebugdrawer);
	def("getDebugdrawer", &i6engine::python::api::getDebugdrawer);
	def("getAppl", &i6engine::python::api::getAppl);
	def("registerDefault", &i6engine::python::api::registerDefault);
	def("stop", &i6engine::python::api::stop);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, uint16_t)) &i6engine::python::api::registerTimer);
	def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, uint16_t)) &i6engine::python::api::registerTimer);
	def("removeTimer", &i6engine::python::api::removeTimer);
	def("removeTimerID", &i6engine::python::api::removeTimerID);
	def("getTimeLeft", &i6engine::python::api::getTimeLeft);
	def("getCurrentTime", &i6engine::python::api::getCurrentTime);
	def("setCurrentTime", &i6engine::python::api::setCurrentTime);
	def("getUUID", &i6engine::python::api::getUUID);
	def("getNewUUID", &i6engine::python::api::getNewUUID);
	def("getType", &i6engine::python::api::getType);

	class_<i6engine::api::Application, i6engine::python::api::ApplicationWrapper>("Application")
		.def(init<>())
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
		.def("setName", &i6engine::api::Application::setName),

	enum_<i6engine::api::KeyCode>("KeyCode")
		.value("KC_UNASSIGNED", i6engine::api::KeyCode::KC_UNASSIGNED)
		.value("KC_ESCAPE", i6engine::api::KeyCode::KC_ESCAPE)
		.value("KC_1", i6engine::api::KeyCode::KC_1)
		.value("KC_2", i6engine::api::KeyCode::KC_2)
		.value("KC_3", i6engine::api::KeyCode::KC_3)
		.value("KC_4", i6engine::api::KeyCode::KC_4)
		.value("KC_5", i6engine::api::KeyCode::KC_5)
		.value("KC_6", i6engine::api::KeyCode::KC_6)
		.value("KC_7", i6engine::api::KeyCode::KC_7)
		.value("KC_8", i6engine::api::KeyCode::KC_8)
		.value("KC_9", i6engine::api::KeyCode::KC_9)
		.value("KC_0", i6engine::api::KeyCode::KC_0)
		.value("KC_MINUS", i6engine::api::KeyCode::KC_MINUS)
		.value("KC_EQUALS", i6engine::api::KeyCode::KC_EQUALS)
		.value("KC_BACK", i6engine::api::KeyCode::KC_BACK)
		.value("KC_TAB", i6engine::api::KeyCode::KC_TAB)
		.value("KC_Q", i6engine::api::KeyCode::KC_Q)
		.value("KC_W", i6engine::api::KeyCode::KC_W)
		.value("KC_E", i6engine::api::KeyCode::KC_E)
		.value("KC_R", i6engine::api::KeyCode::KC_R)
		.value("KC_T", i6engine::api::KeyCode::KC_T)
		.value("KC_Y", i6engine::api::KeyCode::KC_Y)
		.value("KC_U", i6engine::api::KeyCode::KC_U)
		.value("KC_I", i6engine::api::KeyCode::KC_I)
		.value("KC_O", i6engine::api::KeyCode::KC_O)
		.value("KC_P", i6engine::api::KeyCode::KC_P)
		.value("KC_LBRACKET", i6engine::api::KeyCode::KC_LBRACKET)
		.value("KC_RBRACKET", i6engine::api::KeyCode::KC_RBRACKET)
		.value("KC_RETURN", i6engine::api::KeyCode::KC_RETURN)
		.value("KC_LCONTROL", i6engine::api::KeyCode::KC_LCONTROL)
		.value("KC_A", i6engine::api::KeyCode::KC_A)
		.value("KC_S", i6engine::api::KeyCode::KC_S)
		.value("KC_D", i6engine::api::KeyCode::KC_D)
		.value("KC_F", i6engine::api::KeyCode::KC_F)
		.value("KC_G", i6engine::api::KeyCode::KC_G)
		.value("KC_H", i6engine::api::KeyCode::KC_H)
		.value("KC_J", i6engine::api::KeyCode::KC_J)
		.value("KC_K", i6engine::api::KeyCode::KC_K)
		.value("KC_L", i6engine::api::KeyCode::KC_L)
		.value("KC_SEMICOLON", i6engine::api::KeyCode::KC_SEMICOLON)
		.value("KC_APOSTROPHE", i6engine::api::KeyCode::KC_APOSTROPHE)
		.value("KC_GRAVE", i6engine::api::KeyCode::KC_GRAVE)
		.value("KC_LSHIFT", i6engine::api::KeyCode::KC_LSHIFT)
		.value("KC_BACKSLASH", i6engine::api::KeyCode::KC_BACKSLASH)
		.value("KC_Z", i6engine::api::KeyCode::KC_Z)
		.value("KC_X", i6engine::api::KeyCode::KC_X)
		.value("KC_C", i6engine::api::KeyCode::KC_C)
		.value("KC_V", i6engine::api::KeyCode::KC_V)
		.value("KC_B", i6engine::api::KeyCode::KC_B)
		.value("KC_N", i6engine::api::KeyCode::KC_N)
		.value("KC_M", i6engine::api::KeyCode::KC_M)
		.value("KC_COMMA", i6engine::api::KeyCode::KC_COMMA)
		.value("KC_PERIOD", i6engine::api::KeyCode::KC_PERIOD)
		.value("KC_SLASH", i6engine::api::KeyCode::KC_SLASH)
		.value("KC_RSHIFT", i6engine::api::KeyCode::KC_RSHIFT)
		.value("KC_MULTIPLY", i6engine::api::KeyCode::KC_MULTIPLY)
		.value("KC_LMENU", i6engine::api::KeyCode::KC_LMENU)
		.value("KC_SPACE", i6engine::api::KeyCode::KC_SPACE)
		.value("KC_CAPITAL", i6engine::api::KeyCode::KC_CAPITAL)
		.value("KC_F1", i6engine::api::KeyCode::KC_F1)
		.value("KC_F2", i6engine::api::KeyCode::KC_F2)
		.value("KC_F3", i6engine::api::KeyCode::KC_F3)
		.value("KC_F4", i6engine::api::KeyCode::KC_F4)
		.value("KC_F5", i6engine::api::KeyCode::KC_F5)
		.value("KC_F6", i6engine::api::KeyCode::KC_F6)
		.value("KC_F7", i6engine::api::KeyCode::KC_F7)
		.value("KC_F8", i6engine::api::KeyCode::KC_F8)
		.value("KC_F9", i6engine::api::KeyCode::KC_F9)
		.value("KC_F10", i6engine::api::KeyCode::KC_F10)
		.value("KC_NUMLOCK", i6engine::api::KeyCode::KC_NUMLOCK)
		.value("KC_SCROLL", i6engine::api::KeyCode::KC_SCROLL)
		.value("KC_NUMPAD7", i6engine::api::KeyCode::KC_NUMPAD7)
		.value("KC_NUMPAD8", i6engine::api::KeyCode::KC_NUMPAD8)
		.value("KC_NUMPAD9", i6engine::api::KeyCode::KC_NUMPAD9)
		.value("KC_SUBTRACT", i6engine::api::KeyCode::KC_SUBTRACT)
		.value("KC_NUMPAD4", i6engine::api::KeyCode::KC_NUMPAD4)
		.value("KC_NUMPAD5", i6engine::api::KeyCode::KC_NUMPAD5)
		.value("KC_NUMPAD6", i6engine::api::KeyCode::KC_NUMPAD6)
		.value("KC_ADD", i6engine::api::KeyCode::KC_ADD)
		.value("KC_NUMPAD1", i6engine::api::KeyCode::KC_NUMPAD1)
		.value("KC_NUMPAD2", i6engine::api::KeyCode::KC_NUMPAD2)
		.value("KC_NUMPAD3", i6engine::api::KeyCode::KC_NUMPAD3)
		.value("KC_NUMPAD0", i6engine::api::KeyCode::KC_NUMPAD0)
		.value("KC_DECIMAL", i6engine::api::KeyCode::KC_DECIMAL)
		.value("KC_OEM_102", i6engine::api::KeyCode::KC_OEM_102)
		.value("KC_F11", i6engine::api::KeyCode::KC_F11)
		.value("KC_F12", i6engine::api::KeyCode::KC_F12)
		.value("KC_F13", i6engine::api::KeyCode::KC_F13)
		.value("KC_F14", i6engine::api::KeyCode::KC_F14)
		.value("KC_F15", i6engine::api::KeyCode::KC_F15)
		.value("KC_KANA", i6engine::api::KeyCode::KC_KANA)
		.value("KC_ABNT_C1", i6engine::api::KeyCode::KC_ABNT_C1)
		.value("KC_CONVERT", i6engine::api::KeyCode::KC_CONVERT)
		.value("KC_NOCONVERT", i6engine::api::KeyCode::KC_NOCONVERT)
		.value("KC_YEN", i6engine::api::KeyCode::KC_YEN)
		.value("KC_ABNT_C2", i6engine::api::KeyCode::KC_ABNT_C2)
		.value("KC_NUMPADEQUALS", i6engine::api::KeyCode::KC_NUMPADEQUALS)
		.value("KC_PREVTRACK", i6engine::api::KeyCode::KC_PREVTRACK)
		.value("KC_AT", i6engine::api::KeyCode::KC_AT)
		.value("KC_COLON", i6engine::api::KeyCode::KC_COLON)
		.value("KC_UNDERLINE", i6engine::api::KeyCode::KC_UNDERLINE)
		.value("KC_KANJI", i6engine::api::KeyCode::KC_KANJI)
		.value("KC_STOP", i6engine::api::KeyCode::KC_STOP)
		.value("KC_AX", i6engine::api::KeyCode::KC_AX)
		.value("KC_UNLABELED", i6engine::api::KeyCode::KC_UNLABELED)
		.value("KC_NEXTTRACK", i6engine::api::KeyCode::KC_NEXTTRACK)
		.value("KC_NUMPADENTER", i6engine::api::KeyCode::KC_NUMPADENTER)
		.value("KC_RCONTROL", i6engine::api::KeyCode::KC_RCONTROL)
		.value("KC_MUTE", i6engine::api::KeyCode::KC_MUTE)
		.value("KC_CALCULATOR", i6engine::api::KeyCode::KC_CALCULATOR)
		.value("KC_PLAYPAUSE", i6engine::api::KeyCode::KC_PLAYPAUSE)
		.value("KC_MEDIASTOP", i6engine::api::KeyCode::KC_MEDIASTOP)
		.value("KC_VOLUMEDOWN", i6engine::api::KeyCode::KC_VOLUMEDOWN)
		.value("KC_VOLUMEUP", i6engine::api::KeyCode::KC_VOLUMEUP)
		.value("KC_WEBHOME", i6engine::api::KeyCode::KC_WEBHOME)
		.value("KC_NUMPADCOMMA", i6engine::api::KeyCode::KC_NUMPADCOMMA)
		.value("KC_DIVIDE", i6engine::api::KeyCode::KC_DIVIDE)
		.value("KC_SYSRQ", i6engine::api::KeyCode::KC_SYSRQ)
		.value("KC_RMENU", i6engine::api::KeyCode::KC_RMENU)
		.value("KC_PAUSE", i6engine::api::KeyCode::KC_PAUSE)
		.value("KC_HOME", i6engine::api::KeyCode::KC_HOME)
		.value("KC_UP", i6engine::api::KeyCode::KC_UP)
		.value("KC_PGUP", i6engine::api::KeyCode::KC_PGUP)
		.value("KC_LEFT", i6engine::api::KeyCode::KC_LEFT)
		.value("KC_RIGHT", i6engine::api::KeyCode::KC_RIGHT)
		.value("KC_END", i6engine::api::KeyCode::KC_END)
		.value("KC_DOWN", i6engine::api::KeyCode::KC_DOWN)
		.value("KC_PGDOWN", i6engine::api::KeyCode::KC_PGDOWN)
		.value("KC_INSERT", i6engine::api::KeyCode::KC_INSERT)
		.value("KC_DELETE", i6engine::api::KeyCode::KC_DELETE)
		.value("KC_LWIN", i6engine::api::KeyCode::KC_LWIN)
		.value("KC_RWIN", i6engine::api::KeyCode::KC_RWIN)
		.value("KC_APPS", i6engine::api::KeyCode::KC_APPS)
		.value("KC_POWER", i6engine::api::KeyCode::KC_POWER)
		.value("KC_SLEEP", i6engine::api::KeyCode::KC_SLEEP)
		.value("KC_WAKE", i6engine::api::KeyCode::KC_WAKE)
		.value("KC_WEBSEARCH", i6engine::api::KeyCode::KC_WEBSEARCH)
		.value("KC_WEBFAVORITES", i6engine::api::KeyCode::KC_WEBFAVORITES)
		.value("KC_WEBREFRESH", i6engine::api::KeyCode::KC_WEBREFRESH)
		.value("KC_WEBSTOP", i6engine::api::KeyCode::KC_WEBSTOP)
		.value("KC_WEBFORWARD", i6engine::api::KeyCode::KC_WEBFORWARD)
		.value("KC_WEBBACK", i6engine::api::KeyCode::KC_WEBBACK)
		.value("KC_MYCOMPUTER", i6engine::api::KeyCode::KC_MYCOMPUTER)
		.value("KC_MAIL", i6engine::api::KeyCode::KC_MAIL)
		.value("KC_MEDIASELECT", i6engine::api::KeyCode::KC_MEDIASELECT);

	enum_<i6engine::api::MouseButtonID>("MouseButtonID")
		.value("MB_Left", i6engine::api::MouseButtonID::MB_Left)
		.value("MB_Right", i6engine::api::MouseButtonID::MB_Right)
		.value("MB_Middle", i6engine::api::MouseButtonID::MB_Middle)
		.value("MB_Button3", i6engine::api::MouseButtonID::MB_Button3)
		.value("MB_Button4", i6engine::api::MouseButtonID::MB_Button4)
		.value("MB_Button5", i6engine::api::MouseButtonID::MB_Button5)
		.value("MB_Button6", i6engine::api::MouseButtonID::MB_Button6)
		.value("MB_Button7", i6engine::api::MouseButtonID::MB_Button7);

	class_<i6engine::api::IDManager, boost::noncopyable>("IDManager", no_init)
		.def("setBounds", &i6engine::api::IDManager::setBounds)
		.def("getID", (int64_t(i6engine::api::IDManager::*)()) &i6engine::api::IDManager::getID)
		.def("getID", (int64_t(i6engine::api::IDManager::*)(uint32_t)) &i6engine::api::IDManager::getID);

	class_<i6engine::api::LanguageManager, boost::noncopyable>("LanguageManager", no_init)
		.def("addCallback", &i6engine::python::api::addLanguageScriptCallback)
		.def("addCallback", &i6engine::python::api::addLanguageFunctionCallback)
		.def("setLanguage", &i6engine::api::LanguageManager::setLanguage);

	class_<i6engine::api::TextManager, boost::noncopyable>("TextManager", no_init)
		.def("getText", &i6engine::api::TextManager::getText);

	class_<i6engine::api::WaynetManager, boost::noncopyable>("WaynetManager", no_init)
		.def("createWaynet", &i6engine::api::WaynetManager::createWaynet)
		.def("getShortestPath", (std::vector<Vec3>(i6engine::api::WaynetManager::*)(const Vec3 &, const std::string &)) &i6engine::api::WaynetManager::getShortestPath)
		.def("showWaynet", &i6engine::api::WaynetManager::showWaynet)
		.def("getNearestWaypoint", &i6engine::api::WaynetManager::getNearestWaypoint);
}
