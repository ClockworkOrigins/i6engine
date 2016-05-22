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

#include "factories/EventHandlerPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoExpireEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoFreezeEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoStopSystemEventHandlerPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific EventHandler Property Window
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(parent, owner, name);
		} else {
			return new widgets::EventHandlerPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific EventHandler Property Window and propagate the attributes from the EventHandlerPropertyWindow
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::EventHandlerPropertyWindow * eventHandlerPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
