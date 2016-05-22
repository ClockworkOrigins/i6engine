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

#include "properties/TimeAndColourListProperty.h"

#include "properties/TimeAndColourProperty.h"

#include <QPushButton>

namespace i6e {
namespace particleEditor {
namespace properties {

	TimeAndColourListProperty::TimeAndColourListProperty(QWidget * par, QString label, QString name) : Property(par, label, name), _layout(nullptr), _timeAndColours() {
		QWidget * widget = new QWidget(this);
		_layout = new QVBoxLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		QPushButton * pb = new QPushButton(QApplication::tr("Add time and colour"), widget);
		_layout->addWidget(pb);

		connect(pb, SIGNAL(clicked()), this, SLOT(addTimeAndColour()));
	}

	TimeAndColourListProperty::~TimeAndColourListProperty() {
	}

	void TimeAndColourListProperty::setTimeAndColourList(std::vector<std::pair<double, Vec4>> value) {
		for (TimeAndColourProperty * v3p : _timeAndColours) {
			_layout->removeWidget(v3p);
			delete v3p;
		}
		_timeAndColours.clear();
		for (std::pair<double, Vec4> & p : value) {
			TimeAndColourProperty * tacp = new TimeAndColourProperty(this, "", "", p);
			_layout->addWidget(tacp);
			_timeAndColours.push_back(tacp);
			connect(tacp, SIGNAL(changed(QString)), this, SLOT(changedValue()));
		}
	}

	std::vector<std::pair<double, Vec4>> TimeAndColourListProperty::getTimeAndColourList() const {
		std::vector<std::pair<double, Vec4>> value;
		for (TimeAndColourProperty * tacp : _timeAndColours) {
			value.push_back(tacp->getTimeAndColour());
		}
		return value;
	}

	void TimeAndColourListProperty::addTimeAndColour() {
		TimeAndColourProperty * v3p = new TimeAndColourProperty(this, "", "", std::make_pair(0.0, Vec4()));
		_layout->addWidget(v3p);
		_timeAndColours.push_back(v3p);
		connect(v3p, SIGNAL(changed(QString)), this, SLOT(changedValue()));
	}

	void TimeAndColourListProperty::changedValue() {
		triggerChangedSignal();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
