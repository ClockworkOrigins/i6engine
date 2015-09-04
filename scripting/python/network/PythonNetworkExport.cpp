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

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/NetworkFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace network {

	i6engine::core::IPKey getIP() {
		return i6engine::api::EngineController::GetSingleton().getNetworkFacade()->getIP();
	}

	std::string getSimpleIP() {
		return i6engine::api::EngineController::GetSingleton().getNetworkFacade()->getSimpleIP();
	}

	bool connect(const core::IPKey remoteIP, const uint16_t localPort) {
		return i6engine::api::EngineController::GetSingleton().getNetworkFacade()->connect(remoteIP, localPort);
	}

	bool listen(const uint16_t localPort) {
		return i6engine::api::EngineController::GetSingleton().getNetworkFacade()->listen(localPort);
	}

	void disconnect() {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->disconnect();
	}

	void enablePing(bool enabled) {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->enablePing(enabled);
	}

	void subscribe(uint32_t channel) {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->subscribe(channel);
	}

	void unsubscribe(uint32_t channel) {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->unsubscribe(channel);
	}

	void publish(uint32_t channel, const i6engine::api::GameMessage::Ptr & msg) {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->publish(channel, msg);
	}

	void resetNetworkSubSystem() {
		i6engine::api::EngineController::GetSingleton().getNetworkFacade()->resetSubSystem();
	}

} /* namespace network */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingNetworkPython) {
	using namespace boost::python;
	def("getIP", &i6engine::python::network::getIP);
	def("getSimpleIP", &i6engine::python::network::getSimpleIP);
	def("connect", &i6engine::python::network::connect);
	def("listen", &i6engine::python::network::listen);
	def("disconnect", &i6engine::python::network::disconnect);
	def("enablePing", &i6engine::python::network::enablePing);
	def("subscribe", &i6engine::python::network::subscribe);
	def("unsubscribe", &i6engine::python::network::unsubscribe);
	def("publish", &i6engine::python::network::publish);
	def("resetNetworkSubSystem", &i6engine::python::network::resetNetworkSubSystem);
}
