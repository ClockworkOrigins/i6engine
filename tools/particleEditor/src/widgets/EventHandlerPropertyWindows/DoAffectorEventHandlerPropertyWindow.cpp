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

#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EventHandlerPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_PRE_POST = QApplication::tr("Perform pre/post processing");

		// Affector component: Doesn't have a property, because this is established by means of a connection

		// Pre/Post Processing: bool
		append(new properties::BoolProperty(this, PRNL_PRE_POST, PRNL_PRE_POST, ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST));
	}

	DoAffectorEventHandlerPropertyWindow::~DoAffectorEventHandlerPropertyWindow() {
	}

	void DoAffectorEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoAffectorEventHandler * doAffectorEventHandler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(eventHandler);

		// Pre/Post Processing: bool
		setBool(PRNL_PRE_POST, doAffectorEventHandler->getPrePost());
	}

	void DoAffectorEventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::DoAffectorEventHandler * handler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_PRE_POST) {
			// Pre/Post Processing: bool
			handler->setPrePost(prop->getBool());
		} else {
			// Update handler with another attribute
			EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
