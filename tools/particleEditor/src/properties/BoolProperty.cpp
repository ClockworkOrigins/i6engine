#include "properties/BoolProperty.h"

#include <QCheckBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	BoolProperty::BoolProperty(QWidget * par, QString label, QString name, bool value) : Property(par, label, name), _value(value), _checkBox(nullptr) {
		_checkBox = new QCheckBox(this);
		_checkBox->setChecked(value);
		horizontalLayout->addWidget(_checkBox);
		connect(_checkBox, SIGNAL(stateChanged(int)), this, SLOT(boolChanged()));
	}

	BoolProperty::~BoolProperty() {
	}

	void BoolProperty::setBool(bool value) {
		_value = value;
		_checkBox->setChecked(value);
	}

	void BoolProperty::boolChanged() {
		if (_value != _checkBox->isChecked()) {
			_value = _checkBox->isChecked();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
