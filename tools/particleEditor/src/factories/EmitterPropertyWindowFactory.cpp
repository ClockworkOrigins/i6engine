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

#include "factories/EmitterPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/EmitterPropertyWindows/BoxEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/CircleEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/LineEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/PointEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/PositionEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/SlaveEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/SphereSurfaceEmitterPropertyWindow.h"
#include "widgets/EmitterPropertyWindows/VertexEmitterPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Emitter Property Window
	*/
	widgets::EmitterPropertyWindow * EmitterPropertyWindowFactory::createEmitterPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_BOX) {
			return new widgets::BoxEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_CIRCLE) {
			return new widgets::CircleEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_LINE) {
			return new widgets::LineEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_MESH_SURFACE) {
			return new widgets::MeshSurfaceEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_POINT) {
			return new widgets::PointEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_POSITION) {
			return new widgets::PositionEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_SLAVE) {
			return new widgets::SlaveEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_SPHERE) {
			return new widgets::SphereSurfaceEmitterPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EMITTER_VERTEX) {
			return new widgets::VertexEmitterPropertyWindow(parent, owner, name);
		} else {
			return new widgets::EmitterPropertyWindow(parent, owner, name);
		}
		return 0;
	}

	/**
	Create a specific Emitter Property Window and propagate the attributes from the EmitterPropertyWindow
	*/
	widgets::EmitterPropertyWindow * EmitterPropertyWindowFactory::createEmitterPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::EmitterPropertyWindow * emitterPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_BOX) {
			return new widgets::BoxEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_CIRCLE) {
			return new widgets::CircleEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_LINE) {
			return new widgets::LineEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_MESH_SURFACE) {
			return new widgets::MeshSurfaceEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_POINT) {
			return new widgets::PointEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_POSITION) {
			return new widgets::PositionEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_SLAVE) {
			return new widgets::SlaveEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_SPHERE) {
			return new widgets::SphereSurfaceEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else if (subType == widgets::CST_EMITTER_VERTEX) {
			return new widgets::VertexEmitterPropertyWindow(emitterPropertyWindow, owner);
		} else {
			return new widgets::EmitterPropertyWindow(emitterPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
