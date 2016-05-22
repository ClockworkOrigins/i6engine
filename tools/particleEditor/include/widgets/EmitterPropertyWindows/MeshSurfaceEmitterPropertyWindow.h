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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_MESH_SURFACE_EMITTER_NAME = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION = "";
	static QString PRNL_MESH_SURFACE_EMITTER_SCALE = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1 = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2 = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS = "";
	static QString PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX = "";

	class MeshSurfaceEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		MeshSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~MeshSurfaceEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;

	private:
		void copyAttributeToEmitter(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__ */
