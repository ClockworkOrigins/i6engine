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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleRenderer;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_RENDERER_TYPE = "";
	static QString PRNL_RENDERER_RENDER_Q_GROUP = "";
	static QString PRNL_RENDERER_SORTING = "";
	static QString PRNL_RENDERER_TEXCOORDS_DEFINE = "";
	static QString PRNL_RENDERER_TEXCOORDS_SET = "";
	static QString PRNL_RENDERER_TEXCOORDS_ROWS = "";
	static QString PRNL_RENDERER_TEXCOORDS_COLUMNS = "";
	static QString PRNL_RENDERER_USE_SOFT_PARTICLES = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_SCALE = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_DELTA = "";
	static QString PRNL_USE_VERTEX_COLOURS = "";
	static QString PRNL_MAX_ELEMENTS = "";

	class RendererPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~RendererPropertyWindow();

		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer);

	protected:
		void changedProperty(properties::Property * prop, QString name) override;
		virtual void copyAttributeToRenderer(properties::Property * prop, QString propertyName);
		void replaceRendererType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__ */
