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

#include "EditorTypeAngle.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>

namespace i6e {
namespace plugins {

	EditorTypeAngleWidget::EditorTypeAngleWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBox = new QDoubleSpinBox(this);
		_spinBox->setMinimum(-360.0);
		_spinBox->setMaximum(360.0);
		layout->addWidget(_spinBox);
		setLayout(layout);
	}

	void EditorTypeAngleWidget::setValues(const std::string & values) {
		_spinBox->setValue(std::stod(values));
	}

	std::string EditorTypeAngleWidget::getValues() const {
		return std::to_string(_spinBox->value());
	}

	void EditorTypeAngleWidget::setReadOnly(bool readOnly) {
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeAngle::createWidget(QWidget * parent) {
		return new EditorTypeAngleWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
