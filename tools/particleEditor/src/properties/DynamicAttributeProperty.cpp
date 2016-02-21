#include "properties/DynamicAttributeProperty.h"

#include "widgets/EmitterPropertyWindow.h"
#include "widgets/PropertyWindow.h"

#include "ParticleUniverseDynamicAttribute.h"

#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	DynamicAttributeProperty::DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value) : Property(par, label, name), _widget(nullptr), _layout(nullptr), _value(value), _widgets(), _comboBox(nullptr), _values() {
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
		_comboBox = nullptr;
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
	}

	void DynamicAttributeProperty::changedDynamicType() {
		if (_comboBox && _comboBox->currentIndex() != _value->getType()) {
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

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
