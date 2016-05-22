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

#include "factories/ExternPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/GravityExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/SphereColliderExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/VortexExternPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Extern Property Window
	*/
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(parent, owner, name);
		} else {
			return new widgets::ExternPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Extern Property Window and propagate the attributes from the ExternPropertyWindow
	*/
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::ExternPropertyWindow * externPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(externPropertyWindow, owner);
		} else {
			return new widgets::ExternPropertyWindow(externPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
