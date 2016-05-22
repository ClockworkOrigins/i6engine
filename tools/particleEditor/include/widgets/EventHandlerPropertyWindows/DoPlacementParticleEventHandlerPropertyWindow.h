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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_NUMBER_OF_PARTICLES = "";
	static QString PRNL_INHERIT_POSITION = "";
	static QString PRNL_INHERIT_DIRECTION = "";
	static QString PRNL_INHERIT_ORIENTATION = "";
	static QString PRNL_INHERIT_TIME_TO_LIVE = "";
	static QString PRNL_INHERIT_MASS = "";
	static QString PRNL_INHERIT_TEXTURE_COORDINATE = "";
	static QString PRNL_INHERIT_COLOUR = "";
	static QString PRNL_INHERIT_PARTICLE_WIDTH = "";
	static QString PRNL_INHERIT_PARTICLE_HEIGHT = "";
	static QString PRNL_INHERIT_PARTICLE_DEPTH = "";

	class DoPlacementParticleEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoPlacementParticleEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoPlacementParticleEventHandlerPropertyWindow();

		void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) override;

	private:
		void copyAttributeToEventHandler(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__ */
