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

#include "properties/DoubleProperty.h"

#include <QDoubleSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	DoubleProperty::DoubleProperty(QWidget * par, QString label, QString name, double value) : Property(par, label, name), _value(value), _doubleSpinBox(nullptr), _changeable(true) {
		_doubleSpinBox = new QDoubleSpinBox(this);
		_doubleSpinBox->setMinimum(-999999);
		_doubleSpinBox->setMaximum(999999);
		_doubleSpinBox->setValue(value);
		horizontalLayout->addWidget(_doubleSpinBox);
		_doubleSpinBox->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);

		connect(_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
	}

	DoubleProperty::~DoubleProperty() {
	}

	void DoubleProperty::setDouble(double value) {
		_changeable = false;
		_value = value;
		_doubleSpinBox->setValue(value);
		_changeable = true;
	}

	void DoubleProperty::valueChanged() {
		if (_changeable) {
			_value = _doubleSpinBox->value();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
