#include "properties/DynamicAttributeProperty.h"

#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	DynamicAttributeProperty::DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value) : Property(par, label, name), _layout(nullptr), _value(value) {
		widgets::DYN_FIXED = "Fixed";
		widgets::DYN_RANDOM = "Random";
		widgets::DYN_CURVED = "Curved";
		widgets::DYN_OSCILLATE = "Oscillate";
		widgets::DYN_CURVED_LINEAR = "Linear";
		widgets::DYN_CURVED_SPLINE = "Spline";

		QWidget * widget = new QWidget(this);
		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		horizontalLayout->addWidget(widget);
		QComboBox * cb = new QComboBox(this);
		QStringList dynamicTypes;
		dynamicTypes.append(widgets::DYN_FIXED);
		dynamicTypes.append(widgets::DYN_RANDOM);
		dynamicTypes.append(widgets::DYN_CURVED);
		dynamicTypes.append(widgets::DYN_OSCILLATE);
		cb->addItems(dynamicTypes);
		_layout->addWidget(new QLabel("Type", widget), 0, 0);
		_layout->addWidget(cb, 0, 1);

		switch (value->getType()) {
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_FIXED: {
			cb->setCurrentIndex(0);
			_layout->addWidget(new QLabel("Value", widget), 1, 0);
			QDoubleSpinBox * sb = new QDoubleSpinBox(widget);
			sb->setMaximum(INT_MAX);
			sb->setValue(value->getValue());
			_layout->addWidget(sb, 1, 1);
			break;
		}
		case ParticleUniverse::DynamicAttribute::DynamicAttributeType::DAT_RANDOM: {
			cb->setCurrentIndex(1);
			_layout->addWidget(new QLabel("Min", widget), 1, 0);
			QDoubleSpinBox * sb = new QDoubleSpinBox(widget);
			sb->setMaximum(INT_MAX);
			sb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(value)->getMin());
			_layout->addWidget(sb, 1, 1);
			_layout->addWidget(new QLabel("Max", widget), 2, 0);
			sb = new QDoubleSpinBox(widget);
			sb->setMaximum(INT_MAX);
			sb->setValue(dynamic_cast<ParticleUniverse::DynamicAttributeRandom *>(value)->getMax());
			_layout->addWidget(sb, 2, 1);
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

	DynamicAttributeProperty::~DynamicAttributeProperty() {
		delete _value;
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
