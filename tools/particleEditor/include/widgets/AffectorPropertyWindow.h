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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleAffector;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {
	
	static QString PRNL_AFFECTOR_TYPE = "";
	static QString PRNL_AFFECTOR_ENABLED = "";
	static QString PRNL_AFFECTOR_POSITION = "";
	static QString PRNL_AFFECTOR_MASS = "";
	static QString PRNL_AFFECTOR_SPECIALISATION = "";
	static QString PRNL_AFFECTOR_SPEC_DEFAULT = "";
	static QString PRNL_AFFECTOR_SPEC_TT_INCREASE = "";
	static QString PRNL_AFFECTOR_SPEC_TT_DECREASE = "";

	class AffectorPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		AffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~AffectorPropertyWindow();

		virtual void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector);

		void changedProperty(properties::Property * prop, QString name) override;

	protected:
		virtual void copyAttributeToAffector(properties::Property * prop, QString propertyName);

	private:
		void replaceAffectorType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__ */
