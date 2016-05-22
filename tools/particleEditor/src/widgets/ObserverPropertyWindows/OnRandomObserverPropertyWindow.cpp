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

#include "widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_RANDOM_VALUE = QApplication::tr("Random threshold");

		// Random Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ON_RANDOM_VALUE, PRNL_ON_RANDOM_VALUE, ParticleUniverse::OnRandomObserver::DEFAULT_THRESHOLD));
	}

	OnRandomObserverPropertyWindow::~OnRandomObserverPropertyWindow() {
	}

	void OnRandomObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnRandomObserver * onRandomObserver = static_cast<ParticleUniverse::OnRandomObserver *>(observer);

		// Random Threshold
		setDouble(PRNL_ON_RANDOM_VALUE, onRandomObserver->getThreshold());
	}

	void OnRandomObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::OnRandomObserver * observer = static_cast<ParticleUniverse::OnRandomObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_ON_RANDOM_VALUE) {
			// Random Threshold: ParticleUniverse::Real
			observer->setThreshold(prop->getDouble());
		} else {
			// Update observer with another attribute
			ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
