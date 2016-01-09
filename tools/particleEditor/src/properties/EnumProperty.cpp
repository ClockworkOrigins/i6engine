#include "properties/EnumProperty.h"

#include <QComboBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	EnumProperty::EnumProperty(QWidget * par, QString label, QString name, QStringList value) : Property(par, label, name), _value(value) {
		horizontalLayout->addWidget(new QComboBox(this));
		dynamic_cast<QComboBox *>(horizontalLayout->itemAt(1)->widget())->addItems(_value);
	}

	EnumProperty::~EnumProperty() {
	}

	void EnumProperty::setCurrentIndex(int index) {
		dynamic_cast<QComboBox *>(horizontalLayout->itemAt(1)->widget())->setCurrentIndex(index);
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
