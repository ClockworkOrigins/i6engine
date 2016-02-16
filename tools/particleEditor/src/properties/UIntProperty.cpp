#include "properties/UIntProperty.h"

#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	UIntProperty::UIntProperty(QWidget * par, QString label, QString name, unsigned int value) : Property(par, label, name), _value(value), _spinBox(nullptr) {
		_spinBox = new QSpinBox(this);
		_spinBox->setMinimum(0);
		_spinBox->setMaximum(INT_MAX);
		_spinBox->setValue(value);
		horizontalLayout->addWidget(_spinBox);
		connect(_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changedValue()));
	}

	UIntProperty::~UIntProperty() {
	}

	void UIntProperty::setUInt(unsigned int value) {
		_value = value;
		_spinBox->setValue(value);
	}

	void UIntProperty::changedValue() {
		if (_value != _spinBox->value()) {
			_value = _spinBox->value();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
