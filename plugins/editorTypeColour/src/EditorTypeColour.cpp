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

	EditorTypeColourWidget::EditorTypeColourWidget(QWidget * parent) : TypeWidgetInterface(parent), _spinBoxR(nullptr), _spinBoxG(nullptr), _spinBoxB(nullptr), _colorButton(nullptr) {
		QLayout * layout = new QHBoxLayout(this);
		_spinBoxR = new QSpinBox(this);
		_spinBoxR->setMinimum(0);
		_spinBoxR->setMaximum(255);
		QLabel * labelX = new QLabel("R", this);
		_spinBoxG = new QSpinBox(this);
		_spinBoxG->setMinimum(0);
		_spinBoxG->setMaximum(255);
		QLabel * labelY = new QLabel("G", this);
		_spinBoxB = new QSpinBox(this);
		_spinBoxB->setMinimum(0);
		_spinBoxB->setMaximum(255);
		QLabel * labelZ = new QLabel("B", this);
		_colorButton = new QPushButton(this);
		connect(_colorButton, SIGNAL(clicked()), this, SLOT(selectColour()));
		layout->addWidget(labelX);
		layout->addWidget(_spinBoxR);
		layout->addWidget(labelY);
		layout->addWidget(_spinBoxG);
		layout->addWidget(labelZ);
		layout->addWidget(_spinBoxB);
		layout->addWidget(_colorButton);
		setLayout(layout);
	}

	void EditorTypeColourWidget::setValues(const std::string & values) {
		Vec3 vec(values);
		_spinBoxR->setValue(vec.getX() * 255);
		_spinBoxG->setValue(vec.getY() * 255);
		_spinBoxB->setValue(vec.getZ() * 255);
		QColor color(_spinBoxR->value(), _spinBoxG->value(), _spinBoxB->value());
		QString styleSheet = _colorButton->styleSheet();
		QString newStyleSheet = styleSheet.replace(QRegExp("background-color: #[0-9A-Fa-f]*;"), "background-color: " + color.name() + ";");
		if (styleSheet == newStyleSheet) {
			newStyleSheet += "background-color: " + color.name() + ";";
		}
		_colorButton->setStyleSheet(newStyleSheet);

	}

	std::string EditorTypeColourWidget::getValues() const {
		return (QString::number(_spinBoxR->value() / 255.0) + " " + QString::number(_spinBoxG->value() / 255.0) + " " + QString::number(_spinBoxB->value() / 255.0)).toStdString();
	}

	void EditorTypeColourWidget::setReadOnly(bool readOnly) {
		_spinBoxR->setReadOnly(readOnly);
		_spinBoxG->setReadOnly(readOnly);
		_spinBoxB->setReadOnly(readOnly);
	}

	void EditorTypeColourWidget::selectColour() {
		QColorDialog dlg;
		dlg.setCurrentColor(QColor(int(_spinBoxR->value()), int(_spinBoxG->value()), int(_spinBoxB->value())));
		dlg.exec();
		QString styleSheet = _colorButton->styleSheet();
		QString newStyleSheet = styleSheet.replace(QRegExp("background-color: #[0-9A-Fa-f]*;"), "background-color: " + dlg.currentColor().name() + ";");
		if (styleSheet == newStyleSheet) {
			newStyleSheet += "background-color: " + dlg.currentColor().name() + ";";
		}
		_colorButton->setStyleSheet(newStyleSheet);
		_spinBoxR->setValue(dlg.currentColor().red());
		_spinBoxG->setValue(dlg.currentColor().green());
		_spinBoxB->setValue(dlg.currentColor().blue());
	}

	editor::plugins::TypeWidgetInterface * EditorTypeColour::createWidget(QWidget * parent) {
		return new EditorTypeColourWidget(parent);
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e.editor.type.TypePluginInterface")
