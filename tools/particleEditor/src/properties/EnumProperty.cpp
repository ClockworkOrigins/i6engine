#include "properties/EnumProperty.h"

#include <QComboBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	EnumProperty::EnumProperty(QWidget * par, QString label, QString name, QStringList value) : Property(par, label, name), _value(value), _currentText(), _currentIndex() {
		_comboBox = new QComboBox(this);
		horizontalLayout->addWidget(_comboBox);
		_comboBox->addItems(_value);
		connect(_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changedValue()));
	}

	EnumProperty::~EnumProperty() {
	}

	void EnumProperty::setCurrentIndex(int index) {
		_currentIndex = index;
		_comboBox->setCurrentIndex(index);
	}

	void EnumProperty::setEnumString(QString value) {
		_currentText = value;
		_comboBox->setCurrentText(value);
	}

	int EnumProperty::getCurrentIndex() const {
		return _comboBox->currentIndex();
	}

	QString EnumProperty::getEnumString() const {
		return _comboBox->currentText();
	}

	void EnumProperty::changedValue() {
		if (_currentIndex != _comboBox->currentIndex() && _currentText != _comboBox->currentText()) {
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
