#include "properties/Property.h"

#include <QLabel>

namespace i6engine {
namespace particleEditor {
namespace properties {

	Property::Property(QWidget * par, QString label, QString name) : QWidget(par), _label(label), _name(name) {
		setupUi(this);
		horizontalLayout->addWidget(new QLabel(label, this));
	}

	Property::~Property() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
