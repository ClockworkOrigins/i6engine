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

#include "widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	OnCountObserverPropertyWindow::OnCountObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_COUNT_COMPARE = QApplication::tr("On count compare");
		PRNL_ON_COUNT_THRESHOLD = QApplication::tr("On count threshold");
		PRNL_COMPARE_LESS_THAN = "Less than";
		PRNL_COMPARE_GREATER_THAN = "Greater than";
		PRNL_COMPARE_EQUALS = "Equals";

		// Count Threshold Compare: List
		QStringList compare;
		compare.append(PRNL_COMPARE_LESS_THAN);
		compare.append(PRNL_COMPARE_GREATER_THAN);
		compare.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_COUNT_COMPARE, PRNL_ON_COUNT_COMPARE, compare));

		// Count Threshold Value: ParticleUniverse::uint
		append(new properties::UIntProperty(this, PRNL_ON_COUNT_THRESHOLD, PRNL_ON_COUNT_THRESHOLD, ParticleUniverse::OnCountObserver::DEFAULT_THRESHOLD));
	}

	OnCountObserverPropertyWindow::~OnCountObserverPropertyWindow() {
	}

	void OnCountObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnCountObserver * onCountObserver = static_cast<ParticleUniverse::OnCountObserver *>(observer);

		// Count Threshold Compare: List
		ParticleUniverse::ComparisionOperator compare = onCountObserver->getCompare();
		QString compareString = PRNL_COMPARE_LESS_THAN;
		if (compare == ParticleUniverse::CO_GREATER_THAN) {
			compareString = PRNL_COMPARE_GREATER_THAN;
		} else if (compare == ParticleUniverse::CO_EQUALS) {
			compareString = PRNL_COMPARE_EQUALS;
		}
		setEnumString(PRNL_ON_COUNT_COMPARE, compareString);

		// Count Threshold Value: ParticleUniverse::uint
		setUint16(PRNL_ON_COUNT_THRESHOLD, onCountObserver->getThreshold());
	}

	void OnCountObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::OnCountObserver * observer = static_cast<ParticleUniverse::OnCountObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_ON_COUNT_COMPARE) {
			// Count Threshold Compare: List
			QString compare = prop->getEnumString();
			if (compare == PRNL_COMPARE_LESS_THAN) {
				observer->setCompare(ParticleUniverse::CO_LESS_THAN);
			} else if (compare == PRNL_COMPARE_GREATER_THAN) {
				observer->setCompare(ParticleUniverse::CO_GREATER_THAN);
			} else if (compare == PRNL_COMPARE_EQUALS) {
				observer->setCompare(ParticleUniverse::CO_EQUALS);
			}
		} else if (propertyName == PRNL_ON_COUNT_THRESHOLD) {
			// Count Threshold Value: ParticleUniverse::uint
			observer->setThreshold(prop->getUInt());
		} else {
			// Update observer with another attribute
			ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
