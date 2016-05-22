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

#include "properties/BoolProperty.h"

#include <QCheckBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	BoolProperty::BoolProperty(QWidget * par, QString label, QString name, bool value) : Property(par, label, name), _value(value), _checkBox(nullptr) {
		_checkBox = new QCheckBox(this);
		_checkBox->setChecked(value);
		horizontalLayout->addWidget(_checkBox);
		connect(_checkBox, SIGNAL(stateChanged(int)), this, SLOT(boolChanged()));
	}

	BoolProperty::~BoolProperty() {
	}

	void BoolProperty::setBool(bool value) {
		_value = value;
		_checkBox->setChecked(value);
	}

	void BoolProperty::boolChanged() {
		if (_value != _checkBox->isChecked()) {
			_value = _checkBox->isChecked();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
