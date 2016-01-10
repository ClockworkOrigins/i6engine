#include "properties/StringProperty.h"

#include <QLineEdit>

namespace i6engine {
namespace particleEditor {
namespace properties {

	StringProperty::StringProperty(QWidget * par, QString label, QString name, QString value) : Property(par, label, name), _value(value), _lineEdit(nullptr) {
		_lineEdit = new QLineEdit(_value, this);
		horizontalLayout->addWidget(_lineEdit);
		connect(_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(stringChanged(QString)));
	}

	StringProperty::~StringProperty() {
	}

	void StringProperty::setString(QString value) {
		_value = value;
		_lineEdit->setText(value);
	}

	void StringProperty::stringChanged(QString value) {
		_value = value;
		triggerChangedSignal();
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
