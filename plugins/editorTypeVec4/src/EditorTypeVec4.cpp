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

#include "EditorTypeVec4.h"

#include "i6engine/math/i6eVector4.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

namespace i6e {
namespace plugins {

	EditorTypeVec4Widget::EditorTypeVec4Widget(QWidget * parent) : TypeWidgetInterface(parent), _spinBoxX(nullptr), _spinBoxY(nullptr), _spinBoxZ(nullptr), _spinBoxW(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBoxW = new QDoubleSpinBox(this);
		QLabel * labelW = new QLabel("W", this);
		_spinBoxX = new QDoubleSpinBox(this);
		QLabel * labelX = new QLabel("X", this);
		_spinBoxY = new QDoubleSpinBox(this);
		QLabel * labelY = new QLabel("Y", this);
		_spinBoxZ = new QDoubleSpinBox(this);
		QLabel * labelZ = new QLabel("Z", this);
		layout->addWidget(labelW);
		layout->addWidget(_spinBoxW);
		layout->addWidget(labelX);
		layout->addWidget(_spinBoxX);
		layout->addWidget(labelY);
		layout->addWidget(_spinBoxY);
		layout->addWidget(labelZ);
		layout->addWidget(_spinBoxZ);
		setLayout(layout);
	}

	void EditorTypeVec4Widget::setValues(const std::string & values) {
		Vec4 vec(values);
		_spinBoxX->setValue(vec.getX());
		_spinBoxY->setValue(vec.getY());
		_spinBoxZ->setValue(vec.getZ());
		_spinBoxW->setValue(vec.getW());
	}

	std::string EditorTypeVec4Widget::getValues() const {
		return (_spinBoxW->text() + " " + _spinBoxX->text() + " " + _spinBoxY->text() + " " + _spinBoxZ->text()).toStdString();
	}

	void EditorTypeVec4Widget::setReadOnly(bool readOnly) {
		_spinBoxX->setReadOnly(readOnly);
		_spinBoxY->setReadOnly(readOnly);
		_spinBoxZ->setReadOnly(readOnly);
		_spinBoxW->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeVec4::createWidget(QWidget * parent) {
		return new EditorTypeVec4Widget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
