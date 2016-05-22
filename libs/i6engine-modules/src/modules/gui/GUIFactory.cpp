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
