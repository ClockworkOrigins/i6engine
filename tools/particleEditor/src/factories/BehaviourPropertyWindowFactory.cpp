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

#include "factories/BehaviourPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/BehaviourPropertyWindows/SlaveBehaviourPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Behaviour Property Window
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(parent, owner, name);
		} else {
			return new widgets::BehaviourPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Behaviour Property Window and propagate the attributes from the BehaviourPropertyWindow
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::BehaviourPropertyWindow * behaviourPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow, owner);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(behaviourPropertyWindow, owner);
		} else {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
