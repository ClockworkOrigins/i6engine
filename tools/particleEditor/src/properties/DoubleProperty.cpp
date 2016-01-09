#include "properties/DoubleProperty.h"

#include <QDoubleSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	DoubleProperty::DoubleProperty(QWidget * par, QString label, QString name, double value) : Property(par, label, name), _value(value), _doubleSpinBox(nullptr) {
		_doubleSpinBox = new QDoubleSpinBox(this);
		_doubleSpinBox->setMaximum(DBL_MAX);
		_doubleSpinBox->setValue(value);
		horizontalLayout->addWidget(_doubleSpinBox);
		_doubleSpinBox->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
	}

	DoubleProperty::~DoubleProperty() {
	}

	void DoubleProperty::setDouble(double value) {
		_value = value;
		_doubleSpinBox->setValue(value);
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
