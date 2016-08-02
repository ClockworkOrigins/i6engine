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

#include "properties/ColourWithAlphaProperty.h"

#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	ColourWithAlphaProperty::ColourWithAlphaProperty(QWidget * par, QString label, QString name, Vec4 value) : Property(par, label, name), _layout(nullptr), _value(value), _spinBoxR(nullptr), _spinBoxG(nullptr), _spinBoxB(nullptr), _spinBoxA(nullptr), _colourButton(nullptr), _set(false) {
		QWidget * widget = new QWidget(this);
		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		_layout->addWidget(new QLabel(QApplication::tr("Colour"), widget), 0, 0);
		QLabel * l = new QLabel("R", this);
		_layout->addWidget(l, 0, 1);
		_spinBoxR = new QSpinBox(this);
		_spinBoxR->setMaximum(255);
		_spinBoxR->setValue(value.getX());
		_layout->addWidget(_spinBoxR, 0, 2);

		l = new QLabel("G", this);
		_layout->addWidget(l, 0, 3);
		_spinBoxG = new QSpinBox(this);
		_spinBoxG->setMaximum(255);
		_spinBoxG->setValue(value.getY());
		_layout->addWidget(_spinBoxG, 0, 4);

		l = new QLabel("B", this);
		_layout->addWidget(l, 0, 5);
		_spinBoxB = new QSpinBox(this);
		_spinBoxB->setMaximum(255);
		_spinBoxB->setValue(value.getZ());
		_layout->addWidget(_spinBoxB, 0, 6);

		l = new QLabel(QApplication::tr("Alpha"), this);
		_layout->addWidget(l, 1, 1);
		_spinBoxA = new QSpinBox(this);
		_spinBoxA->setMaximum(255);
		_spinBoxA->setValue(value.getW());
		_layout->addWidget(_spinBoxA, 1, 2);

		_colourButton = new QPushButton(this);
		connect(_colourButton, SIGNAL(clicked()), this, SLOT(selectColour()));
		_layout->addWidget(_colourButton, 1, 3);

		connect(_spinBoxR, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
		connect(_spinBoxG, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
		connect(_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
		connect(_spinBoxA, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
	}

	ColourWithAlphaProperty::~ColourWithAlphaProperty() {
	}

	void ColourWithAlphaProperty::setColourWithAlpha(Vec4 value) {
		// set is used to know in valueChanged when value is not edited via GUI but set from code to avoid notification
		_set = true;
		_value = value;
		_spinBoxR->setValue(value.getX());
		_spinBoxG->setValue(value.getY());
		_spinBoxB->setValue(value.getZ());
		_spinBoxA->setValue(value.getW());
		_set = false;
		QColor color(_spinBoxR->value(), _spinBoxG->value(), _spinBoxB->value(), _spinBoxA->value());
		QString styleSheet = _colourButton->styleSheet();
		QString newStyleSheet = styleSheet.replace(QRegExp("background-color: #[0-9A-Fa-f]*;"), "background-color: " + color.name() + ";");
		if (styleSheet == newStyleSheet) {
			newStyleSheet += "background-color: " + color.name() + ";";
		}
		_colourButton->setStyleSheet(newStyleSheet);
	}

	void ColourWithAlphaProperty::valueChanged() {
		if (!_set) {
			_value.setW(_spinBoxA->value());
			_value.setX(_spinBoxR->value());
			_value.setY(_spinBoxG->value());
			_value.setZ(_spinBoxB->value());
			triggerChangedSignal();
		}
	}

	void ColourWithAlphaProperty::selectColour() {
		QColorDialog dlg;
		dlg.setCurrentColor(QColor(int(_spinBoxR->value()), int(_spinBoxG->value()), int(_spinBoxB->value()), int(_spinBoxA->value())));
		dlg.exec();
		QString styleSheet = _colourButton->styleSheet();
		QString newStyleSheet = styleSheet.replace(QRegExp("background-color: #[0-9A-Fa-f]*;"), "background-color: " + dlg.currentColor().name() + ";");
		if (styleSheet == newStyleSheet) {
			newStyleSheet += "background-color: " + dlg.currentColor().name() + ";";
		}
		_colourButton->setStyleSheet(newStyleSheet);
		_set = true;
		_spinBoxR->setValue(dlg.currentColor().red());
		_spinBoxG->setValue(dlg.currentColor().green());
		_spinBoxB->setValue(dlg.currentColor().blue());
		_spinBoxA->setValue(dlg.currentColor().alpha());
		_set = false;
		valueChanged();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
