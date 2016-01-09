#include "properties/DynamicAttributeProperty.h"

#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	DynamicAttributeProperty::DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value) : Property(par, label, name), _widget(nullptr), _layout(nullptr), _value(value), _widgets() {
		widgets::DYN_FIXED = "Fixed";
		widgets::DYN_RANDOM = "Random";
		widgets::DYN_CURVED = "Curved";
		widgets::DYN_OSCILLATE = "Oscillate";
		widgets::DYN_CURVED_LINEAR = "Linear";
		widgets::DYN_CURVED_SPLINE = "Spline";

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
		for (QWidget * w : _widgets) {
			_layout->removeWidget(w);
			delete w;
		}
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
	}

	void DynamicAttributeProperty::createGUI() {
		QComboBox * cb = new QComboBox(this);
		QStringList dynamicTypes;
		dynamicTypes.append(widgets::DYN_FIXED);
		dynamicTypes.append(widgets::DYN_RANDOM);
		dynamicTypes.append(widgets::DYN_CURVED);
		dynamicTypes.append(widgets::DYN_OSCILLATE);
		cb->addItems(dynamicTypes);
		QLabel * l = new QLabel("Type", _widget);
		_layout->addWidget(l, 0, 0);
		_layout->addWidget(cb, 0, 1);
		_widgets.push_back(cb);
		_widgets.push_back(l);

		switch (_value->getType()) {
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
			cb->setCurrentIndex(0);
			l = new QLabel("Value", _widget);
			_layout->addWidget(l, 1, 0);
			QDoubleSpinBox * dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(_value->getValue());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 1, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
			cb->setCurrentIndex(1);
			l = new QLabel("Min", _widget);
			_layout->addWidget(l, 1, 0);
			QDoubleSpinBox * dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->getMin());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 1, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			l = new QLabel("Max", _widget);
			_layout->addWidget(l, 2, 0);
			dsb = new QDoubleSpinBox(_widget);
			dsb->setMaximum(DBL_MAX);
			dsb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(_value)->getMax());
			dsb->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
			_layout->addWidget(dsb, 2, 1);
			_widgets.push_back(dsb);
			_widgets.push_back(l);
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_CURVED: {
			cb->setCurrentIndex(2);
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_OSCILLATE: {
			cb->setCurrentIndex(3);
			break;
		}
		default: {
			break;
		}
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
