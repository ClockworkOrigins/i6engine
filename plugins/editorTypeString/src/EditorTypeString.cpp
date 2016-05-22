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

#include "EditorTypeString.h"

#include <QHBoxLayout>
#include <QLineEdit>

namespace i6e {
namespace plugins {

	EditorTypeStringWidget::EditorTypeStringWidget(QWidget * parent) : TypeWidgetInterface(parent), _lineEdit(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_lineEdit = new QLineEdit(this);
		layout->addWidget(_lineEdit);
		setLayout(layout);
	}

	void EditorTypeStringWidget::setValues(const std::string & values) {
		_lineEdit->setText(QString::fromStdString(values));
	}

	std::string EditorTypeStringWidget::getValues() const {
		return _lineEdit->text().toStdString();
	}

	void EditorTypeStringWidget::setReadOnly(bool readOnly) {
		_lineEdit->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeString::createWidget(QWidget * parent) {
		return new EditorTypeStringWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
