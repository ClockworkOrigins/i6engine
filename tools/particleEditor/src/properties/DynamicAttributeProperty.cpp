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

#include "properties/DynamicAttributeProperty.h"

#include <cfloat>

#include "widgets/EmitterPropertyWindow.h"
#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	DynamicAttributeProperty::DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value) : Property(par, label, name), _widget(nullptr), _layout(nullptr), _value(value), _widgets(), _comboBox(nullptr), _values(), _interpolationTypeBox(nullptr), _controlPoints(), _setting(false) {
		widgets::DYN_FIXED = QApplication::tr("Fixed");
		widgets::DYN_RANDOM = QApplication::tr("Random");
		widgets::DYN_CURVED = QApplication::tr("Curved");
		widgets::DYN_OSCILLATE = QApplication::tr("Oscillate");
		widgets::DYN_CURVED_LINEAR = QApplication::tr("Linear");
		widgets::DYN_CURVED_SPLINE = QApplication::tr("Spline");

		_widget = new QWidget(this);
		_layout = new QGridLayout(_widget);
		_widget->setLayout(_layout);
		horizontalLayout->addWidget(_widget);

		createGUI();
	}

	DynamicAttributeProperty::~DynamicAttributeProperty() {
		delete _value;
	}

	void DynamicAttributeProperty::setDynamicAttribute(ParticleUniverse::DynamicAttribute * value) {
		_setting = true;
		_comboBox = nullptr;
		_interpolationTypeBox = nullptr;
		for (QWidget * w : _widgets) {
			_layout->removeWidget(w);
			delete w;
		}
		_widgets.clear();
		_values.clear();
		delete _value;
		switch (value->getType()) {
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
			_value = new ParticleUniverse::DynamicAttributeFixed();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
			_value = new ParticleUniverse::DynamicAttributeRandom();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
			_value = new ParticleUniverse::DynamicAttributeCurved();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
			_value = new ParticleUniverse::DynamicAttributeOscillate();
			break;
		}
		default: {
			_value = nullptr;
			break;
		}
		}
		value->copyAttributesTo(_value);
		createGUI();
		_setting = false;
	}

	void DynamicAttributeProperty::changedDynamicType() {
		if (_comboBox && _comboBox->currentIndex() != _value->getType() && !_setting) {
			ParticleUniverse::DynamicAttribute * value = nullptr;
			switch (ParticleUniverse::DynamicAttribute::DynamicAttributeType(_comboBox->currentIndex())) {
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
				value = new ParticleUniverse::DynamicAttributeFixed();
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
				value = new ParticleUniverse::DynamicAttributeRandom();
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
				value = new ParticleUniverse::DynamicAttributeCurved();
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
				value = new ParticleUniverse::DynamicAttributeOscillate();
				break;
			}
			default: {
				_value = nullptr;
				break;
			}
			}
			setDynamicAttribute(value);
			delete value;
			triggerChangedSignal();
		}
	}

	void DynamicAttributeProperty::changedValue() {
		if (!_setting) {
			switch (_value->getType()) {
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
				dynamic_cast<ParticleUniverse::DynamicAttributeFixed *>(_value)->setValue(_values[PropertyTypes::Value]->value());
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
				dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->setMin(_values[PropertyTypes::MinValue]->value());
				dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->setMax(_values[PropertyTypes::MaxValue]->value());
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
				dynamic_cast<ParticleUniverse::DynamicAttributeCurved *>(_value)->setInterpolationType(ParticleUniverse::InterpolationType(_interpolationTypeBox->currentIndex()));
				dynamic_cast<ParticleUniverse::DynamicAttributeCurved *>(_value)->removeAllControlPoints();
				for (auto & p : _controlPoints) {
					dynamic_cast<ParticleUniverse::DynamicAttributeCurved *>(_value)->addControlPoint(p.first->value(), p.second->value());
				}
				break;
			}
			case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
				break;
			}
			default: {
				break;
			}
			}
			triggerChangedSignal();
		}
	}

	ParticleUniverse::DynamicAttribute * DynamicAttributeProperty::getDynamicAttribute() const {
		ParticleUniverse::DynamicAttribute * value = nullptr;
		switch (_value->getType()) {
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
			value = new ParticleUniverse::DynamicAttributeFixed();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
			value = new ParticleUniverse::DynamicAttributeRandom();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
			value = new ParticleUniverse::DynamicAttributeCurved();
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
			value = new ParticleUniverse::DynamicAttributeOscillate();
			break;
		}
		default: {
			break;
		}
		}
		_value->copyAttributesTo(value);
		return value;
	}

	void DynamicAttributeProperty::createGUI() {
		widgets::PRNL_EMITTER_VELOCITY = QApplication::tr("Velocity");
		_comboBox = new QComboBox(this);
		QStringList dynamicTypes;
		dynamicTypes.append(widgets::DYN_FIXED);
		dynamicTypes.append(widgets::DYN_RANDOM);
		dynamicTypes.append(widgets::DYN_CURVED);
		dynamicTypes.append(widgets::DYN_OSCILLATE);
		_comboBox->addItems(dynamicTypes);
		QLabel * l = new QLabel(QApplication::tr("Type"), _widget);
		_layout->addWidget(l, 0, 0);
		_layout->addWidget(_comboBox, 0, 1);
		_widgets.push_back(_comboBox);
		_widgets.push_back(l);

		switch (_value->getType()) {
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
			_comboBox->setCurrentIndex(0);
			l = new QLabel(QApplication::tr("Value"), _widget);
			_layout->addWidget(l, 1, 0);
			QDoubleSpinBox * dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(_value->getValue());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 1, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			_values.insert(std::make_pair(PropertyTypes::Value, dsb));
			connect(dsb, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
			_comboBox->setCurrentIndex(1);
			l = new QLabel(QApplication::tr("Min"), _widget);
			_layout->addWidget(l, 1, 0);
			QDoubleSpinBox * dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->getMin());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 1, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			_values.insert(std::make_pair(PropertyTypes::MinValue, dsb));
			connect(dsb, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
			l = new QLabel(QApplication::tr("Max"), _widget);
			_layout->addWidget(l, 2, 0);
			dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->getMax());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 2, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			_values.insert(std::make_pair(PropertyTypes::MaxValue, dsb));
			connect(dsb, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
			_comboBox->setCurrentIndex(2);
			QComboBox * comboBox = new QComboBox(this);
			connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedDynamicType()), Qt::QueuedConnection);
			QStringList interpolationTypes;
			interpolationTypes.append(widgets::DYN_CURVED_LINEAR);
			interpolationTypes.append(widgets::DYN_CURVED_SPLINE);
			comboBox->addItems(interpolationTypes);
			_interpolationTypeBox = comboBox;
			ParticleUniverse::DynamicAttributeCurved * dynAttr = dynamic_cast<ParticleUniverse::DynamicAttributeCurved *>(_value);
			if (dynAttr->getInterpolationType() == ParticleUniverse::InterpolationType::IT_LINEAR) {
				comboBox->setCurrentIndex(0);
			} else if (dynAttr->getInterpolationType() == ParticleUniverse::InterpolationType::IT_SPLINE) {
				comboBox->setCurrentIndex(1);
			}
			l = new QLabel(QApplication::tr("Interpolation Type"), _widget);
			_layout->addWidget(l, 1, 0);
			_layout->addWidget(comboBox, 1, 1);
			_widgets.push_back(comboBox);
			_widgets.push_back(l);
			QPushButton * pb = new QPushButton(QApplication::tr("Add control point"), _widget);
			_layout->addWidget(pb, 2, 0);
			connect(pb, SIGNAL(clicked()), this, SLOT(addControlPoint()));
			for (auto p : dynAttr->getControlPoints()) {
				addControlPoint();
				_controlPoints.back().first->setValue(p.x);
				_controlPoints.back().second->setValue(p.y);
			}
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
			_comboBox->setCurrentIndex(3);
			break;
		}
		default: {
			break;
		}
		}
		connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedDynamicType()), Qt::QueuedConnection);
	}

	void DynamicAttributeProperty::addControlPoint() {
		QDoubleSpinBox * dsbTimepoint = new QDoubleSpinBox(_widget);
		dsbTimepoint->setMinimum(0.0);
		dsbTimepoint->setMaximum(1.0);
		QDoubleSpinBox * dsbValue = new QDoubleSpinBox(_widget);
		dsbValue->setMinimum(-999999);
		dsbValue->setMaximum(999999);
		_layout->addWidget(dsbTimepoint, 3 + int(_controlPoints.size()), 0);
		_layout->addWidget(dsbValue, 3 + int(_controlPoints.size()), 1);
		_widgets.push_back(dsbTimepoint);
		_widgets.push_back(dsbValue);
		connect(dsbTimepoint, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
		connect(dsbValue, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
		_controlPoints.push_back(std::make_pair(dsbTimepoint, dsbValue));
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
