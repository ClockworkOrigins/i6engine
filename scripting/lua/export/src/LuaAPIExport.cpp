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

	class KeyCode {
	};

	class MouseButtonID {
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

		def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, uint32_t)) &i6engine::lua::api::getCurrentTime),
		def("registerSubSystem", (void(*)(const std::string &, i6engine::core::ModuleController *, const std::set<i6engine::lua::core::Subsystem::Type> &)) &i6engine::lua::api::getCurrentTime),
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
		def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, const std::string &, bool, uint16_t)) &i6engine::lua::api::registerTimer),
		def("registerTimer", (uint64_t(*)(uint64_t, const std::string &, bool, uint16_t)) &i6engine::lua::api::registerTimer),
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

	class_<i6engine::lua::api::KeyCode>("KeyCode")
		.def(constructor<>())
		.enum_("KeyCode")
		[
			value("KC_UNASSIGNED", int(i6engine::api::KeyCode::KC_UNASSIGNED)),
			value("KC_ESCAPE", int(i6engine::api::KeyCode::KC_ESCAPE)),
			value("KC_1", int(i6engine::api::KeyCode::KC_1)),
			value("KC_2", int(i6engine::api::KeyCode::KC_2)),
			value("KC_3", int(i6engine::api::KeyCode::KC_3)),
			value("KC_4", int(i6engine::api::KeyCode::KC_4)),
			value("KC_5", int(i6engine::api::KeyCode::KC_5)),
			value("KC_6", int(i6engine::api::KeyCode::KC_6)),
			value("KC_7", int(i6engine::api::KeyCode::KC_7)),
			value("KC_8", int(i6engine::api::KeyCode::KC_8)),
			value("KC_9", int(i6engine::api::KeyCode::KC_9)),
			value("KC_0", int(i6engine::api::KeyCode::KC_0)),
			value("KC_MINUS", int(i6engine::api::KeyCode::KC_MINUS)),
			value("KC_EQUALS", int(i6engine::api::KeyCode::KC_EQUALS)),
			value("KC_BACK", int(i6engine::api::KeyCode::KC_BACK)),
			value("KC_TAB", int(i6engine::api::KeyCode::KC_TAB)),
			value("KC_Q", int(i6engine::api::KeyCode::KC_Q)),
			value("KC_W", int(i6engine::api::KeyCode::KC_W)),
			value("KC_E", int(i6engine::api::KeyCode::KC_E)),
			value("KC_R", int(i6engine::api::KeyCode::KC_R)),
			value("KC_T", int(i6engine::api::KeyCode::KC_T)),
			value("KC_Y", int(i6engine::api::KeyCode::KC_Y)),
			value("KC_U", int(i6engine::api::KeyCode::KC_U)),
			value("KC_I", int(i6engine::api::KeyCode::KC_I)),
			value("KC_O", int(i6engine::api::KeyCode::KC_O)),
			value("KC_P", int(i6engine::api::KeyCode::KC_P)),
			value("KC_LBRACKET", int(i6engine::api::KeyCode::KC_LBRACKET)),
			value("KC_RBRACKET", int(i6engine::api::KeyCode::KC_RBRACKET)),
			value("KC_RETURN", int(i6engine::api::KeyCode::KC_RETURN)),
			value("KC_LCONTROL", int(i6engine::api::KeyCode::KC_LCONTROL)),
			value("KC_A", int(i6engine::api::KeyCode::KC_A)),
			value("KC_S", int(i6engine::api::KeyCode::KC_S)),
			value("KC_D", int(i6engine::api::KeyCode::KC_D)),
			value("KC_F", int(i6engine::api::KeyCode::KC_F)),
			value("KC_G", int(i6engine::api::KeyCode::KC_G)),
			value("KC_H", int(i6engine::api::KeyCode::KC_H)),
			value("KC_J", int(i6engine::api::KeyCode::KC_J)),
			value("KC_K", int(i6engine::api::KeyCode::KC_K)),
			value("KC_L", int(i6engine::api::KeyCode::KC_L)),
			value("KC_SEMICOLON", int(i6engine::api::KeyCode::KC_SEMICOLON)),
			value("KC_APOSTROPHE", int(i6engine::api::KeyCode::KC_APOSTROPHE)),
			value("KC_GRAVE", int(i6engine::api::KeyCode::KC_GRAVE)),
			value("KC_LSHIFT", int(i6engine::api::KeyCode::KC_LSHIFT)),
			value("KC_BACKSLASH", int(i6engine::api::KeyCode::KC_BACKSLASH)),
			value("KC_Z", int(i6engine::api::KeyCode::KC_Z)),
			value("KC_X", int(i6engine::api::KeyCode::KC_X)),
			value("KC_C", int(i6engine::api::KeyCode::KC_C)),
			value("KC_V", int(i6engine::api::KeyCode::KC_V)),
			value("KC_B", int(i6engine::api::KeyCode::KC_B)),
			value("KC_N", int(i6engine::api::KeyCode::KC_N)),
			value("KC_M", int(i6engine::api::KeyCode::KC_M)),
			value("KC_COMMA", int(i6engine::api::KeyCode::KC_COMMA)),
			value("KC_PERIOD", int(i6engine::api::KeyCode::KC_PERIOD)),
			value("KC_SLASH", int(i6engine::api::KeyCode::KC_SLASH)),
			value("KC_RSHIFT", int(i6engine::api::KeyCode::KC_RSHIFT)),
			value("KC_MULTIPLY", int(i6engine::api::KeyCode::KC_MULTIPLY)),
			value("KC_LMENU", int(i6engine::api::KeyCode::KC_LMENU)),
			value("KC_SPACE", int(i6engine::api::KeyCode::KC_SPACE)),
			value("KC_CAPITAL", int(i6engine::api::KeyCode::KC_CAPITAL)),
			value("KC_F1", int(i6engine::api::KeyCode::KC_F1)),
			value("KC_F2", int(i6engine::api::KeyCode::KC_F2)),
			value("KC_F3", int(i6engine::api::KeyCode::KC_F3)),
			value("KC_F4", int(i6engine::api::KeyCode::KC_F4)),
			value("KC_F5", int(i6engine::api::KeyCode::KC_F5)),
			value("KC_F6", int(i6engine::api::KeyCode::KC_F6)),
			value("KC_F7", int(i6engine::api::KeyCode::KC_F7)),
			value("KC_F8", int(i6engine::api::KeyCode::KC_F8)),
			value("KC_F9", int(i6engine::api::KeyCode::KC_F9)),
			value("KC_F10", int(i6engine::api::KeyCode::KC_F10)),
			value("KC_NUMLOCK", int(i6engine::api::KeyCode::KC_NUMLOCK)),
			value("KC_SCROLL", int(i6engine::api::KeyCode::KC_SCROLL)),
			value("KC_NUMPAD7", int(i6engine::api::KeyCode::KC_NUMPAD7)),
			value("KC_NUMPAD8", int(i6engine::api::KeyCode::KC_NUMPAD8)),
			value("KC_NUMPAD9", int(i6engine::api::KeyCode::KC_NUMPAD9)),
			value("KC_SUBTRACT", int(i6engine::api::KeyCode::KC_SUBTRACT)),
			value("KC_NUMPAD4", int(i6engine::api::KeyCode::KC_NUMPAD4)),
			value("KC_NUMPAD5", int(i6engine::api::KeyCode::KC_NUMPAD5)),
			value("KC_NUMPAD6", int(i6engine::api::KeyCode::KC_NUMPAD6)),
			value("KC_ADD", int(i6engine::api::KeyCode::KC_ADD)),
			value("KC_NUMPAD1", int(i6engine::api::KeyCode::KC_NUMPAD1)),
			value("KC_NUMPAD2", int(i6engine::api::KeyCode::KC_NUMPAD2)),
			value("KC_NUMPAD3", int(i6engine::api::KeyCode::KC_NUMPAD3)),
			value("KC_NUMPAD0", int(i6engine::api::KeyCode::KC_NUMPAD0)),
			value("KC_DECIMAL", int(i6engine::api::KeyCode::KC_DECIMAL)),
			value("KC_OEM_102", int(i6engine::api::KeyCode::KC_OEM_102)),
			value("KC_F11", int(i6engine::api::KeyCode::KC_F11)),
			value("KC_F12", int(i6engine::api::KeyCode::KC_F12)),
			value("KC_F13", int(i6engine::api::KeyCode::KC_F13)),
			value("KC_F14", int(i6engine::api::KeyCode::KC_F14)),
			value("KC_F15", int(i6engine::api::KeyCode::KC_F15)),
			value("KC_KANA", int(i6engine::api::KeyCode::KC_KANA)),
			value("KC_ABNT_C1", int(i6engine::api::KeyCode::KC_ABNT_C1)),
			value("KC_CONVERT", int(i6engine::api::KeyCode::KC_CONVERT)),
			value("KC_NOCONVERT", int(i6engine::api::KeyCode::KC_NOCONVERT)),
			value("KC_YEN", int(i6engine::api::KeyCode::KC_YEN)),
			value("KC_ABNT_C2", int(i6engine::api::KeyCode::KC_ABNT_C2)),
			value("KC_NUMPADEQUALS", int(i6engine::api::KeyCode::KC_NUMPADEQUALS)),
			value("KC_PREVTRACK", int(i6engine::api::KeyCode::KC_PREVTRACK)),
			value("KC_AT", int(i6engine::api::KeyCode::KC_AT)),
			value("KC_COLON", int(i6engine::api::KeyCode::KC_COLON)),
			value("KC_UNDERLINE", int(i6engine::api::KeyCode::KC_UNDERLINE)),
			value("KC_KANJI", int(i6engine::api::KeyCode::KC_KANJI)),
			value("KC_STOP", int(i6engine::api::KeyCode::KC_STOP)),
			value("KC_AX", int(i6engine::api::KeyCode::KC_AX)),
			value("KC_UNLABELED", int(i6engine::api::KeyCode::KC_UNLABELED)),
			value("KC_NEXTTRACK", int(i6engine::api::KeyCode::KC_NEXTTRACK)),
			value("KC_NUMPADENTER", int(i6engine::api::KeyCode::KC_NUMPADENTER)),
			value("KC_RCONTROL", int(i6engine::api::KeyCode::KC_RCONTROL)),
			value("KC_MUTE", int(i6engine::api::KeyCode::KC_MUTE)),
			value("KC_CALCULATOR", int(i6engine::api::KeyCode::KC_CALCULATOR)),
			value("KC_PLAYPAUSE", int(i6engine::api::KeyCode::KC_PLAYPAUSE)),
			value("KC_MEDIASTOP", int(i6engine::api::KeyCode::KC_MEDIASTOP)),
			value("KC_VOLUMEDOWN", int(i6engine::api::KeyCode::KC_VOLUMEDOWN)),
			value("KC_VOLUMEUP", int(i6engine::api::KeyCode::KC_VOLUMEUP)),
			value("KC_WEBHOME", int(i6engine::api::KeyCode::KC_WEBHOME)),
			value("KC_NUMPADCOMMA", int(i6engine::api::KeyCode::KC_NUMPADCOMMA)),
			value("KC_DIVIDE", int(i6engine::api::KeyCode::KC_DIVIDE)),
			value("KC_SYSRQ", int(i6engine::api::KeyCode::KC_SYSRQ)),
			value("KC_RMENU", int(i6engine::api::KeyCode::KC_RMENU)),
			value("KC_PAUSE", int(i6engine::api::KeyCode::KC_PAUSE)),
			value("KC_HOME", int(i6engine::api::KeyCode::KC_HOME)),
			value("KC_UP", int(i6engine::api::KeyCode::KC_UP)),
			value("KC_PGUP", int(i6engine::api::KeyCode::KC_PGUP)),
			value("KC_LEFT", int(i6engine::api::KeyCode::KC_LEFT)),
			value("KC_RIGHT", int(i6engine::api::KeyCode::KC_RIGHT)),
			value("KC_END", int(i6engine::api::KeyCode::KC_END)),
			value("KC_DOWN", int(i6engine::api::KeyCode::KC_DOWN)),
			value("KC_PGDOWN", int(i6engine::api::KeyCode::KC_PGDOWN)),
			value("KC_INSERT", int(i6engine::api::KeyCode::KC_INSERT)),
			value("KC_DELETE", int(i6engine::api::KeyCode::KC_DELETE)),
			value("KC_LWIN", int(i6engine::api::KeyCode::KC_LWIN)),
			value("KC_RWIN", int(i6engine::api::KeyCode::KC_RWIN)),
			value("KC_APPS", int(i6engine::api::KeyCode::KC_APPS)),
			value("KC_POWER", int(i6engine::api::KeyCode::KC_POWER)),
			value("KC_SLEEP", int(i6engine::api::KeyCode::KC_SLEEP)),
			value("KC_WAKE", int(i6engine::api::KeyCode::KC_WAKE)),
			value("KC_WEBSEARCH", int(i6engine::api::KeyCode::KC_WEBSEARCH)),
			value("KC_WEBFAVORITES", int(i6engine::api::KeyCode::KC_WEBFAVORITES)),
			value("KC_WEBREFRESH", int(i6engine::api::KeyCode::KC_WEBREFRESH)),
			value("KC_WEBSTOP", int(i6engine::api::KeyCode::KC_WEBSTOP)),
			value("KC_WEBFORWARD", int(i6engine::api::KeyCode::KC_WEBFORWARD)),
			value("KC_WEBBACK", int(i6engine::api::KeyCode::KC_WEBBACK)),
			value("KC_MYCOMPUTER", int(i6engine::api::KeyCode::KC_MYCOMPUTER)),
			value("KC_MAIL", int(i6engine::api::KeyCode::KC_MAIL)),
			value("KC_MEDIASELECT", int(i6engine::api::KeyCode::KC_MEDIASELECT))
		],
		
	class_<i6engine::lua::api::MouseButtonID>("MouseButtonID")
		.def(constructor<>())
		.enum_("MouseButtonID")
		[
			value("MB_Left", int(i6engine::api::MouseButtonID::MB_Left)),
			value("MB_Right", int(i6engine::api::MouseButtonID::MB_Right)),
			value("MB_Middle", int(i6engine::api::MouseButtonID::MB_Middle)),
			value("MB_Button3", int(i6engine::api::MouseButtonID::MB_Button3)),
			value("MB_Button4", int(i6engine::api::MouseButtonID::MB_Button4)),
			value("MB_Button5", int(i6engine::api::MouseButtonID::MB_Button5)),
			value("MB_Button6", int(i6engine::api::MouseButtonID::MB_Button6)),
			value("MB_Button7", int(i6engine::api::MouseButtonID::MB_Button7))
		],

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
