#include "properties/StringProperty.h"

#include <QLineEdit>

namespace i6engine {
namespace particleEditor {
namespace properties {

	StringProperty::StringProperty(QWidget * par, QString label, QString name, QString value) : Property(par, label, name), _value(value) {
		horizontalLayout->addWidget(new QLineEdit(_value, this));
	}

	StringProperty::~StringProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
