#include "properties/UIntProperty.h"

#include <QSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	UIntProperty::UIntProperty(QWidget * par, QString label, QString name, unsigned int value) : Property(par, label, name), _value(value) {
		QSpinBox * dsb = new QSpinBox(this);
		dsb->setMaximum(1000000);
		dsb->setValue(value);
		horizontalLayout->addWidget(dsb);
	}

	UIntProperty::~UIntProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
