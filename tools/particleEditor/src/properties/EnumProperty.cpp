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

#include "properties/EnumProperty.h"

#include <QComboBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	EnumProperty::EnumProperty(QWidget * par, QString label, QString name, QStringList value) : Property(par, label, name), _value(value), _currentText(), _currentIndex() {
		_comboBox = new QComboBox(this);
		horizontalLayout->addWidget(_comboBox);
		_comboBox->addItems(_value);
		connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedValue()));
	}

	EnumProperty::~EnumProperty() {
	}

	void EnumProperty::setCurrentIndex(int index) {
		_currentIndex = index;
		_comboBox->setCurrentIndex(index);
	}

	void EnumProperty::setEnumString(QString value) {
		_currentText = value;
		_comboBox->setCurrentText(value);
	}

	int EnumProperty::getCurrentIndex() const {
		return _comboBox->currentIndex();
	}

	QString EnumProperty::getEnumString() const {
		return _comboBox->currentText();
	}

	void EnumProperty::changedValue() {
		if (_currentIndex != _comboBox->currentIndex() && _currentText != _comboBox->currentText()) {
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
