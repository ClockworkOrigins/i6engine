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

#include "i6engine/api/facades/ScriptingFacade.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/ScriptingConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	ScriptingFacade::ScriptingFacade() {
	}

	ScriptingFacade::~ScriptingFacade() {
	}

	void ScriptingFacade::callScript(const std::string & file, const std::string & func) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ScriptingMessageType, scripting::ScrCall, core::Method::Update, new scripting::Scripting_Call_Update(file, func), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ScriptingFacade::callScript(const std::string & file, const std::string & func, const int64_t id) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ScriptingMessageType, scripting::ScrCallID, core::Method::Update, new scripting::Scripting_CallID_Update(file, func, id), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ScriptingFacade::callScript(const std::string & file, const std::string & func, const int64_t ownID, const int64_t otherID) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ScriptingMessageType, scripting::ScrCallID2, core::Method::Update, new scripting::Scripting_CallID2_Update(file, func, ownID, otherID), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ScriptingFacade::callScript(const std::string & file, const std::string & func, const int64_t ownID, const double otherID) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ScriptingMessageType, scripting::ScrCallID2Double, core::Method::Update, new scripting::Scripting_CallID2Double_Update(file, func, ownID, otherID), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void ScriptingFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::ScriptingMessageType, scripting::ScrReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6engine */
