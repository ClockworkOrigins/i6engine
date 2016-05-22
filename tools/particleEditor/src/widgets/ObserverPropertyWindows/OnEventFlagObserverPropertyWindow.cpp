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

#include "widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.h"

#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleObservers/ParticleUniverseOnEventFlagObserver.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	OnEventFlagObserverPropertyWindow::OnEventFlagObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_EVENTFLAG_FLAG = QApplication::tr("Event Flag");

		// Event Flag: ParticleUniverse::uint32
		append(new properties::UIntProperty(this, PRNL_ON_EVENTFLAG_FLAG, PRNL_ON_EVENTFLAG_FLAG, ParticleUniverse::OnEventFlagObserver::DEFAULT_EVENT_FLAG));
	}

	OnEventFlagObserverPropertyWindow::~OnEventFlagObserverPropertyWindow() {
	}

	void OnEventFlagObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnEventFlagObserver * onEventFlagObserver = static_cast<ParticleUniverse::OnEventFlagObserver *>(observer);

		// Event Flag: ParticleUniverse::uint32
		setUint16(PRNL_ON_EVENTFLAG_FLAG, onEventFlagObserver->getEventFlag());
	}

	void OnEventFlagObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::OnEventFlagObserver * observer = static_cast<ParticleUniverse::OnEventFlagObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_ON_EVENTFLAG_FLAG) {
			// Event Flag: ParticleUniverse::uint32
			observer->setEventFlag(prop->getUInt());
		} else {
			// Update observer with another attribute
			ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
