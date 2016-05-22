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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleEmitter;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_EMITTER_TYPE = "";
	static QString PRNL_EMITTER_ENABLED = "";
	static QString PRNL_EMITTER_POSITION = "";
	static QString PRNL_EMITTER_KEEP_LOCAL = "";
	static QString PRNL_EMITTER_DIRECTION = "";
	static QString PRNL_EMITTER_ORIENTATION = "";
	static QString PRNL_EMITTER_ORIENTATION_START = "";
	static QString PRNL_EMITTER_ORIENTATION_END = "";
	static QString PRNL_EMITTER_VELOCITY = "";
	static QString PRNL_EMITTER_DURATION = "";
	static QString PRNL_EMITTER_REPEAT_DELAY = "";
	static QString PRNL_EMITTER_ANGLE = "";
	static QString PRNL_EMITTER_EMISSION_RATE = "";
	static QString PRNL_EMITTER_TIME_TO_LIVE = "";
	static QString PRNL_EMITTER_MASS = "";
	static QString PRNL_EMITTER_TEXTURE_COORD = "";
	static QString PRNL_EMITTER_TEXTURE_COORD_START = "";
	static QString PRNL_EMITTER_TEXTURE_COORD_END = "";
	static QString PRNL_EMITTER_COLOUR = "";
	static QString PRNL_EMITTER_COLOUR_RANGE_START = "";
	static QString PRNL_EMITTER_COLOUR_RANGE_END = "";
	static QString PRNL_EMITTER_ALL_PARTICLE_DIM = "";
	static QString PRNL_EMITTER_PARTICLE_WIDTH = "";
	static QString PRNL_EMITTER_PARTICLE_HEIGHT = "";
	static QString PRNL_EMITTER_PARTICLE_DEPTH = "";
	static QString PRNL_EMITTER_AUTO_DIRECTION = "";
	static QString PRNL_EMITTER_FORCE_EMISSION = "";

	class EmitterPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~EmitterPropertyWindow();

		virtual void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter);

		void changedProperty(properties::Property * prop, QString name) override;

	protected:
		virtual void copyAttributeToEmitter(properties::Property * prop, QString propertyName);

	private:
		void replaceEmitterType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EMITTERPROPERTYWINDOW_H__ */
