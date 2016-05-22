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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_SYSTEM_CATEGORY = "";
	static QString PRNL_SYSTEM_KEEP_LOCAL = "";
	static QString PRNL_SYSTEM_ITERATION_INTERVAL = "";
	static QString PRNL_SYSTEM_FIXED_TIMEOUT = "";
	static QString PRNL_SYSTEM_NONVIS_UPDATE_TIMEOUT = "";
	static QString PRNL_SYSTEM_LOD_DISTANCES = "";
	static QString PRNL_SYSTEM_SMOOTH_LOD = "";
	static QString PRNL_SYSTEM_FAST_FORWARD = "";
	static QString PRNL_SYSTEM_FAST_FORWARD_TIME = "";
	static QString PRNL_SYSTEM_FAST_FORWARD_INTERVAL = "";
	static QString PRNL_SYSTEM_MAIN_CAMERA_NAME = "";
	static QString PRNL_SYSTEM_SCALE_VELOCITY = "";
	static QString PRNL_SYSTEM_SCALE_TIME = "";
	static QString PRNL_SYSTEM_SCALE = "";
	static QString PRNL_SYSTEM_TIGHT_BOUNDING_BOX = "";

	class SystemPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SystemPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name);

		/**
		 * \brief destructor
		 */
		~SystemPropertyWindow();

		virtual void copyAttributesFromSystem(ParticleUniverse::ParticleSystem * system);

	protected:
		void changedProperty(properties::Property * prop, QString name) override;
		void copyAttributeToSystem(properties::Property * prop, QString propertyName);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SYSTEMPROPERTYWINDOW_H__ */
