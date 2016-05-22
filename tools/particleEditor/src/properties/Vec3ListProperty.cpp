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

#include "properties/Vec3ListProperty.h"

#include "properties/Vec3Property.h"

#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QPushButton>

namespace i6e {
namespace particleEditor {
namespace properties {

	Vec3ListProperty::Vec3ListProperty(QWidget * par, QString label, QString name) : Property(par, label, name), _layout(nullptr), _positions() {
		QWidget * widget = new QWidget(this);
		_layout = new QVBoxLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		QPushButton * pb = new QPushButton(widgets::PRNL_PATHFOLLOWER_POSITION, widget);
		_layout->addWidget(pb);

		connect(pb, SIGNAL(clicked()), this, SLOT(addPosition()));
	}

	Vec3ListProperty::~Vec3ListProperty() {
	}

	void Vec3ListProperty::setVector3List(std::vector<ParticleUniverse::Vector3> value) {
		for (Vec3Property * v3p : _positions) {
			_layout->removeWidget(v3p);
			delete v3p;
		}
		_positions.clear();
		for (ParticleUniverse::Vector3 & v3 : value) {
			Vec3Property * v3p = new Vec3Property(this, widgets::PRNL_POSITION, widgets::PRNL_POSITION, v3);
			_layout->addWidget(v3p);
			_positions.push_back(v3p);
			connect(v3p, SIGNAL(changed(QString)), this, SLOT(changedValue()));
		}
	}

	std::vector<ParticleUniverse::Vector3> Vec3ListProperty::getVector3List() const {
		std::vector<ParticleUniverse::Vector3> value;
		for (Vec3Property * v3p : _positions) {
			value.push_back(v3p->getVector3());
		}
		return value;
	}

	void Vec3ListProperty::addPosition() {
		Vec3Property * v3p = new Vec3Property(this, widgets::PRNL_POSITION, widgets::PRNL_POSITION, ParticleUniverse::Vector3::ZERO);
		_layout->addWidget(v3p);
		_positions.push_back(v3p);
		connect(v3p, SIGNAL(changed(QString)), this, SLOT(changedValue()));
	}

	void Vec3ListProperty::changedValue() {
		triggerChangedSignal();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
