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

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "boost/python.hpp"

namespace i6engine {
namespace python {
namespace gui {

	void addImageset(const std::string & imageset) {
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addImageset(imageset);
	}

	void addPrint(const std::string & name, const std::string & type, double x, double y, const std::string & message, api::gui::Alignment alignment, int64_t lifetime) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->addPrint(name, type, x, y, message, alignment, lifetime);
	}

	void addText(const std::string & name, const std::string & text) {
		i6engine::api::EngineController::GetSingletonPtr()->getGUIFacade()->addTextToWidget(name, text);
	}

	void setSize(const std::string & name, double w, double h) {
		api::EngineController::GetSingletonPtr()->getGUIFacade()->setSize(name, w, h);
	}

} /* namespace gui */
} /* namespace python */
} /* namespace i6engine */

BOOST_PYTHON_MODULE(ScriptingGUIPython) {
	using namespace boost::python;

	def("addImageset", &i6engine::python::gui::addImageset);
	def("addPrint", &i6engine::python::gui::addPrint);
	def("addText", &i6engine::python::gui::addText);
	def("setSize", &i6engine::python::gui::setSize);

	enum_<i6engine::api::gui::Alignment>("Alignment")
		.value("Left", i6engine::api::gui::Alignment::Left)
		.value("Center", i6engine::api::gui::Alignment::Center)
		.value("Right", i6engine::api::gui::Alignment::Right)
		.export_values();
}
