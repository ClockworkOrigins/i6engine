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

#include "LuaNetworkExport.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/NetworkFacade.h"

namespace i6e {
namespace lua {
namespace network {

	i6e::core::IPKey getIP() {
		return i6e::api::EngineController::GetSingleton().getNetworkFacade()->getIP();
	}

	std::string getSimpleIP() {
		return i6e::api::EngineController::GetSingleton().getNetworkFacade()->getSimpleIP();
	}

	bool connect(const core::IPKey remoteIP, const uint16_t localPort) {
		return i6e::api::EngineController::GetSingleton().getNetworkFacade()->connect(remoteIP, localPort);
	}

	bool listen(const uint16_t localPort) {
		return i6e::api::EngineController::GetSingleton().getNetworkFacade()->listen(localPort);
	}

	void disconnect() {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->disconnect();
	}

	void enablePing(bool enabled) {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->enablePing(enabled);
	}

	void subscribe(uint32_t channel) {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->subscribe(channel);
	}

	void unsubscribe(uint32_t channel) {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->unsubscribe(channel);
	}

	void publish(uint32_t channel, const i6e::api::GameMessage::Ptr & msg) {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->publish(channel, msg);
	}

	void resetNetworkSubSystem() {
		i6e::api::EngineController::GetSingleton().getNetworkFacade()->resetSubSystem();
	}

} /* namespace network */
} /* namespace lua */
} /* namespace i6e */

using namespace luabind;

scope registerNetwork() {
	return
		def("getIP", &i6e::lua::network::getIP),
		def("getSimpleIP", &i6e::lua::network::getSimpleIP),
		def("connect", &i6e::lua::network::connect),
		def("listen", &i6e::lua::network::listen),
		def("disconnect", &i6e::lua::network::disconnect),
		def("enablePing", &i6e::lua::network::enablePing),
		def("subscribe", &i6e::lua::network::subscribe),
		def("unsubscribe", &i6e::lua::network::unsubscribe),
		def("publish", &i6e::lua::network::publish),
		def("resetNetworkSubSystem", &i6e::lua::network::resetNetworkSubSystem)
		;
}
