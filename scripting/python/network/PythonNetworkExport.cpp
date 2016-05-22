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

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/facades/NetworkFacade.h"

#include "boost/python.hpp"

namespace i6e {
namespace python {
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
} /* namespace python */
} /* namespace i6e */

BOOST_PYTHON_MODULE(ScriptingNetworkPython) {
	using namespace boost::python;
	def("getIP", &i6e::python::network::getIP);
	def("getSimpleIP", &i6e::python::network::getSimpleIP);
	def("connect", &i6e::python::network::connect);
	def("listen", &i6e::python::network::listen);
	def("disconnect", &i6e::python::network::disconnect);
	def("enablePing", &i6e::python::network::enablePing);
	def("subscribe", &i6e::python::network::subscribe);
	def("unsubscribe", &i6e::python::network::unsubscribe);
	def("publish", &i6e::python::network::publish);
	def("resetNetworkSubSystem", &i6e::python::network::resetNetworkSubSystem);
}
