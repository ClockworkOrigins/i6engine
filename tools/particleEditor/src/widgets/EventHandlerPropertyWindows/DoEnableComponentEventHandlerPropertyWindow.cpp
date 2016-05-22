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

#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EventHandlerPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ENABLE_COMPONENT_TRUE_FALSE = QApplication::tr("Enable component");

		// Component to enable/disable: Realised by means of a connection

		// Enable/Disable: bool
		append(new properties::BoolProperty(this, PRNL_ENABLE_COMPONENT_TRUE_FALSE, PRNL_ENABLE_COMPONENT_TRUE_FALSE, true));
	}

	DoEnableComponentEventHandlerPropertyWindow::~DoEnableComponentEventHandlerPropertyWindow() {
	}

	void DoEnableComponentEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoEnableComponentEventHandler * doEnableComponentEventHandler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(eventHandler);

		// Enable/Disable: bool
		setBool(PRNL_ENABLE_COMPONENT_TRUE_FALSE, doEnableComponentEventHandler->isComponentEnabled());
	}

	void DoEnableComponentEventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::DoEnableComponentEventHandler * handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_ENABLE_COMPONENT_TRUE_FALSE) {
			// Enable/Disable: bool
			handler->setComponentEnabled(prop->getBool());
		} else {
			// Update handler with another attribute
			EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
