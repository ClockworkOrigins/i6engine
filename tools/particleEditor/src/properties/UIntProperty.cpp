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

#include "properties/UIntProperty.h"

#include <QSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	UIntProperty::UIntProperty(QWidget * par, QString label, QString name, unsigned int value) : Property(par, label, name), _value(value), _spinBox(nullptr) {
		_spinBox = new QSpinBox(this);
		_spinBox->setMinimum(0);
		_spinBox->setMaximum(INT_MAX);
		_spinBox->setValue(value);
		horizontalLayout->addWidget(_spinBox);
		connect(_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));
	}

	UIntProperty::~UIntProperty() {
	}

	void UIntProperty::setUInt(unsigned int value) {
		_value = value;
		_spinBox->setValue(value);
	}

	void UIntProperty::changedValue() {
		if (_value != static_cast<unsigned int>(_spinBox->value())) {
			_value = _spinBox->value();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
