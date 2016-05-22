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

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

namespace i6e {
namespace plugins {

	EditorTypeVec3Widget::EditorTypeVec3Widget(QWidget * parent) : TypeWidgetInterface(parent), _lineEditX(nullptr), _lineEditY(nullptr), _lineEditZ(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_lineEditX = new QLineEdit(this);
		QLabel * labelX = new QLabel("X", this);
		_lineEditY = new QLineEdit(this);
		QLabel * labelY = new QLabel("Y", this);
		_lineEditZ = new QLineEdit(this);
		QLabel * labelZ = new QLabel("Z", this);
		layout->addWidget(labelX);
		layout->addWidget(_lineEditX);
		layout->addWidget(labelY);
		layout->addWidget(_lineEditY);
		layout->addWidget(labelZ);
		layout->addWidget(_lineEditZ);
		setLayout(layout);
	}

	void EditorTypeVec3Widget::setValues(const std::string & values) {
		Vec3 vec(values);
		_lineEditX->setText(QString::number(vec.getX()));
		_lineEditY->setText(QString::number(vec.getY()));
		_lineEditZ->setText(QString::number(vec.getZ()));
	}

	std::string EditorTypeVec3Widget::getValues() const {
		return (_lineEditX->text() + " " + _lineEditY->text() + " " + _lineEditZ->text()).toStdString();
	}

	void EditorTypeVec3Widget::setReadOnly(bool readOnly) {
		_lineEditX->setReadOnly(readOnly);
		_lineEditY->setReadOnly(readOnly);
		_lineEditZ->setReadOnly(readOnly);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeVec3::createWidget(QWidget * parent) {
		return new EditorTypeVec3Widget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
