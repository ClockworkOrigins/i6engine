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

#include "EditorTypeBool.h"

#include <QHBoxLayout>
#include <QCheckBox>

namespace i6e {
namespace plugins {

	EditorTypeBoolWidget::EditorTypeBoolWidget(QWidget * parent) : TypeWidgetInterface(parent), _checkBox(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_checkBox = new QCheckBox(this);
		layout->addWidget(_checkBox);
		setLayout(layout);
	}

	void EditorTypeBoolWidget::setValues(const std::string & values) {
		_checkBox->setChecked(std::stoi(values));
	}

	std::string EditorTypeBoolWidget::getValues() const {
		return std::to_string(_checkBox->isChecked());
	}

	void EditorTypeBoolWidget::setReadOnly(bool readOnly) {
		_checkBox->setEnabled(!readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeBool::createWidget(QWidget * parent) {
		return new EditorTypeBoolWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
