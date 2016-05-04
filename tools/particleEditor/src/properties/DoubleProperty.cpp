#include "properties/DoubleProperty.h"

#include <QDoubleSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	DoubleProperty::DoubleProperty(QWidget * par, QString label, QString name, double value) : Property(par, label, name), _value(value), _doubleSpinBox(nullptr), _changeable(true) {
		_doubleSpinBox = new QDoubleSpinBox(this);
		_doubleSpinBox->setMinimum(-999999);
		_doubleSpinBox->setMaximum(999999);
		_doubleSpinBox->setValue(value);
		horizontalLayout->addWidget(_doubleSpinBox);
		_doubleSpinBox->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);

		connect(_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(valueChanged()));
	}

	DoubleProperty::~DoubleProperty() {
	}

	void DoubleProperty::setDouble(double value) {
		_changeable = false;
		_value = value;
		_doubleSpinBox->setValue(value);
		_changeable = true;
	}

	void DoubleProperty::valueChanged() {
		if (_changeable) {
			_value = _doubleSpinBox->value();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
