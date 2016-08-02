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

#include "EditorTypeVec3.h"

#include "i6engine/math/i6eVector.h"

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

namespace i6e {
namespace plugins {

	EditorTypeVec3Widget::EditorTypeVec3Widget(QWidget * parent) : TypeWidgetInterface(parent), _spinBoxX(nullptr), _spinBoxY(nullptr), _spinBoxZ(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBoxX = new QDoubleSpinBox(this);
		QLabel * labelX = new QLabel("X", this);
		_spinBoxY = new QDoubleSpinBox(this);
		QLabel * labelY = new QLabel("Y", this);
		_spinBoxZ = new QDoubleSpinBox(this);
		QLabel * labelZ = new QLabel("Z", this);
		layout->addWidget(labelX);
		layout->addWidget(_spinBoxX);
		layout->addWidget(labelY);
		layout->addWidget(_spinBoxY);
		layout->addWidget(labelZ);
		layout->addWidget(_spinBoxZ);
		setLayout(layout);
	}

	void EditorTypeVec3Widget::setValues(const std::string & values) {
		Vec3 vec(values);
		_spinBoxX->setValue(vec.getX());
		_spinBoxY->setValue(vec.getY());
		_spinBoxZ->setValue(vec.getZ());
	}

	std::string EditorTypeVec3Widget::getValues() const {
		return (_spinBoxX->text() + " " + _spinBoxY->text() + " " + _spinBoxZ->text()).toStdString();
	}

	void EditorTypeVec3Widget::setReadOnly(bool readOnly) {
		_spinBoxX->setReadOnly(readOnly);
		_spinBoxY->setReadOnly(readOnly);
		_spinBoxZ->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeVec3::createWidget(QWidget * parent) {
		return new EditorTypeVec3Widget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
