#include "properties/EnumProperty.h"

#include <QComboBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	EnumProperty::EnumProperty(QWidget * par, QString label, QString name, QStringList value) : Property(par, label, name), _value(value) {
		_comboBox = new QComboBox(this);
		horizontalLayout->addWidget(_comboBox);
		_comboBox->addItems(_value);
		connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedValue()));
	}

	EnumProperty::~EnumProperty() {
	}

	void EnumProperty::setCurrentIndex(int index) {
		_comboBox->setCurrentIndex(index);
	}

	void EnumProperty::setEnumString(QString value) {
		_comboBox->setCurrentText(value);
	}

	int EnumProperty::getCurrentIndex() const {
		return _comboBox->currentIndex();
	}

	QString EnumProperty::getEnumString() const {
		return _comboBox->currentText();
	}

	void EnumProperty::changedValue() {
		triggerChangedSignal();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
