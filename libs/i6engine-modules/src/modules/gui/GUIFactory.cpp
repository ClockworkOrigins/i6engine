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

#include "i6engine/modules/gui/GUIFactory.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/gui/GUIWidget.h"

namespace i6e {
namespace modules {

	GUIFactory::GUIFactory() {
	}

	GUIFactory::~GUIFactory() {
	}

	api::GUIWidget * GUIFactory::createGUIWidget(const std::string & name, const std::string & type, const std::string & style) const {
		if (_templates.find(type) == _templates.end()) {
			ISIXE_THROW_API("GUIFactory", "Invalid template: " << type);
		}
		return _templates.find(type)->second(name, style);
	}

	void GUIFactory::registerGUIWidgetTemplate(const std::string & name, const GUIWidget_factory & createFunc) {
		if (_templates.find(name) != _templates.end()) {
			ISIXE_THROW_API("GUIFactory", "Double template: " << name);
		}
		_templates[name] = createFunc;
	}

} /* namespace modules */
} /* namespace i6e */
