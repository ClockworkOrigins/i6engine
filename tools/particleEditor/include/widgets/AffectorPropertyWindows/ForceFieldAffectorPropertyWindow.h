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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_AFFECTOR_FORCEFIELD_FF_TYPE = "";
	static QString FORCEFIELD_REALTIME = "";
	static QString FORCEFIELD_MATRIX = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_DELTA = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FORCE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_OCTAVES = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FREQUENCY = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FF_SIZE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_X = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_MOVEMENT = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY = "";

	class ForceFieldAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ForceFieldAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~ForceFieldAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;

	private:
		void copyAttributeToAffector(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__ */
