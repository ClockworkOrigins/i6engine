#include "properties/BoolProperty.h"

#include <QCheckBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	BoolProperty::BoolProperty(QWidget * par, QString label, QString name, bool value) : Property(par, label, name), _value(value) {
		QCheckBox * cb = new QCheckBox(this);
		cb->setChecked(value);
		horizontalLayout->addWidget(cb);
	}

	BoolProperty::~BoolProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
