#include "properties/DoubleProperty.h"

#include <QDoubleSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	DoubleProperty::DoubleProperty(QWidget * par, QString label, QString name, double value) : Property(par, label, name), _value(value) {
		QDoubleSpinBox * dsb = new QDoubleSpinBox(this);
		dsb->setMaximum(1000000);
		dsb->setValue(value);
		horizontalLayout->addWidget(dsb);
	}

	DoubleProperty::~DoubleProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
