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

#include "EditorTypeColour.h"

#include "i6engine/math/i6eVector.h"

#include <QColorDialog>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

namespace i6e {
namespace plugins {

	EditorTypeColourWidget::EditorTypeColourWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBoxX(nullptr), _spinBoxY(nullptr), _spinBoxZ(nullptr), _colorButton(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBoxX = new QSpinBox(this);
		_spinBoxX->setMinimum(0);
		_spinBoxX->setMaximum(255);
		QLabel * labelX = new QLabel("X", this);
		_spinBoxY = new QSpinBox(this);
		_spinBoxY->setMinimum(0);
		_spinBoxY->setMaximum(255);
		QLabel * labelY = new QLabel("Y", this);
		_spinBoxZ = new QSpinBox(this);
		_spinBoxZ->setMinimum(0);
		_spinBoxZ->setMaximum(255);
		QLabel * labelZ = new QLabel("Z", this);
		_colorButton = new QPushButton(this);
		connect(_colorButton, SIGNAL(clicked()), this, SLOT(selectColour()));
		layout->addWidget(labelX);
		layout->addWidget(_spinBoxX);
		layout->addWidget(labelY);
		layout->addWidget(_spinBoxY);
		layout->addWidget(labelZ);
		layout->addWidget(_spinBoxZ);
		layout->addWidget(_colorButton);
		setLayout(layout);
	}

	void EditorTypeColourWidget::setValues(const std::string & values) {
		Vec3 vec(values);
		_spinBoxX->setValue(vec.getX() * 255);
		_spinBoxY->setValue(vec.getY() * 255);
		_spinBoxZ->setValue(vec.getZ() * 255);
	}

	std::string EditorTypeColourWidget::getValues() const {
		return (QString::number(_spinBoxX->value() / 255.0) + " " + QString::number(_spinBoxY->value() / 255.0) + " " + QString::number(_spinBoxZ->value() / 255.0)).toStdString();
	}

	void EditorTypeColourWidget::setReadOnly(bool readOnly) {
		_spinBoxX->setReadOnly(readOnly);
		_spinBoxY->setReadOnly(readOnly);
		_spinBoxZ->setReadOnly(readOnly);
	}

	void EditorTypeColourWidget::selectColour() {
		QColorDialog dlg;
		dlg.setCurrentColor(QColor(int(_spinBoxX->value()), int(_spinBoxY->value()), int(_spinBoxZ->value())));
		dlg.exec();
		QPalette pal = _colorButton->palette();
		pal.setColor(QPalette::ButtonText, dlg.currentColor());
		_colorButton->setPalette(pal);
	}

	editor::plugins::TypeWidgetInterface * EditorTypeColour::createWidget(QWidget * parent) {
		return new EditorTypeColourWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
