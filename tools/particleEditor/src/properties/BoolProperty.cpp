#include "properties/BoolProperty.h"

#include <QCheckBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	BoolProperty::BoolProperty(QWidget * par, QString label, QString name, bool value) : Property(par, label, name), _value(value), _checkBox(nullptr) {
		_checkBox = new QCheckBox(this);
		_checkBox->setChecked(value);
		horizontalLayout->addWidget(_checkBox);
	}

	BoolProperty::~BoolProperty() {
	}

	void BoolProperty::setBool(bool value) {
		_value = value;
		_checkBox->setChecked(value);
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
