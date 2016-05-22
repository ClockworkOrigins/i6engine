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

#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__

#include "properties/Property.h"

class QComboBox;
class QDoubleSpinBox;

namespace i6e {
namespace particleEditor {
namespace properties {

	class DynamicAttributeProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value);

		/**
		 * \brief destructor
		 */
		~DynamicAttributeProperty();

		void setDynamicAttribute(ParticleUniverse::DynamicAttribute * value) override;

		ParticleUniverse::DynamicAttribute * getDynamicAttribute() const override;

	private slots:
		void changedDynamicType();
		void changedValue();
		void addControlPoint();

	private:
		enum PropertyTypes {
			Value,
			MinValue,
			MaxValue
		};
		QWidget * _widget;
		QGridLayout * _layout;
		ParticleUniverse::DynamicAttribute * _value;
		std::vector<QWidget *> _widgets;
		QComboBox * _comboBox;
		std::map<PropertyTypes, QDoubleSpinBox *> _values;
		QComboBox * _interpolationTypeBox;
		std::vector<std::pair<QDoubleSpinBox *, QDoubleSpinBox *>> _controlPoints;
		bool _setting;

		void createGUI();
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__ */
