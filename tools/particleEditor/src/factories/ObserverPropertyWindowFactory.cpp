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

#include "factories/ObserverPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/ObserverPropertyWindows/OnClearObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnCollisionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnEmissionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnExpireObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnQuotaObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnTimeObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Observer Property Window
	*/
	widgets::ObserverPropertyWindow * ObserverPropertyWindowFactory::createObserverPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_CLEAR) {
			return new widgets::OnClearObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_COLLISION) {
			return new widgets::OnCollisionObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_COUNT) {
			return new widgets::OnCountObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EMISSION) {
			return new widgets::OnEmissionObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EVENTFLAG) {
			return new widgets::OnEventFlagObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EXPIRE) {
			return new widgets::OnExpireObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_POSITION) {
			return new widgets::OnPositionObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_QUOTA) {
			return new widgets::OnQuotaObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_RANDOM) {
			return new widgets::OnRandomObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_TIME) {
			return new widgets::OnTimeObserverPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_OBSERVER_ON_VELOCITY) {
			return new widgets::OnVelocityObserverPropertyWindow(parent, owner, name);
		} else {
			return new widgets::ObserverPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Observer Property Window and propagate the attributes from the ObserverPropertyWindow
	*/
	widgets::ObserverPropertyWindow * ObserverPropertyWindowFactory::createObserverPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::ObserverPropertyWindow * observerPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_CLEAR) {
			return new widgets::OnClearObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_COLLISION) {
			return new widgets::OnCollisionObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_COUNT) {
			return new widgets::OnCountObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_EMISSION) {
			return new widgets::OnEmissionObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_EVENTFLAG) {
			return new widgets::OnEventFlagObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_EXPIRE) {
			return new widgets::OnExpireObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_POSITION) {
			return new widgets::OnPositionObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_QUOTA) {
			return new widgets::OnQuotaObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_RANDOM) {
			return new widgets::OnRandomObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_TIME) {
			return new widgets::OnTimeObserverPropertyWindow(observerPropertyWindow, owner);
		} else if (subType == widgets::CST_OBSERVER_ON_VELOCITY) {
			return new widgets::OnVelocityObserverPropertyWindow(observerPropertyWindow, owner);
		} else {
			return new widgets::ObserverPropertyWindow(observerPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
