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

#include "EditorTypeInteger.h"

#include <QHBoxLayout>
#include <QSpinBox>

namespace i6e {
namespace plugins {

	EditorTypeIntegerWidget::EditorTypeIntegerWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBox = new QSpinBox(this);
		layout->addWidget(_spinBox);
		setLayout(layout);
	}

	void EditorTypeIntegerWidget::setValues(const std::string & values) {
		_spinBox->setValue(std::stoi(values));
	}

	std::string EditorTypeIntegerWidget::getValues() const {
		return std::to_string(_spinBox->value());
	}

	void EditorTypeIntegerWidget::setReadOnly(bool readOnly) {
		_spinBox->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeInteger::createWidget(QWidget * parent) {
		return new EditorTypeIntegerWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
