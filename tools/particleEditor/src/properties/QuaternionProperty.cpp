#include "properties/QuaternionProperty.h"

#include "i6engine/math/i6eVector.h"

#include "properties/Vec3Property.h"

#include "widgets/PropertyWindow.h"

#include <QLabel>
#include <QDoubleSpinBox>

namespace i6engine {
namespace particleEditor {
namespace properties {

	QuaternionProperty::QuaternionProperty(QWidget * par, QString label, QString name, Quaternion value) : Property(par, label, name), _layout(nullptr), _value(value) {
		QWidget * widget = new QWidget(this);
		Vec3 axis;
		double angle;
		value.toAxisAngle(axis, angle);

		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		_layout->addWidget(new Vec3Property(widget, "Axis", "Axis", axis.toOgre()), 0, 0);

		QLabel * l = new QLabel("Angle", widget);
		_layout->addWidget(l, 1, 0);
		QDoubleSpinBox * dsb = new QDoubleSpinBox(this);
		dsb->setMinimum(-360.0);
		dsb->setMaximum(360.0);
		dsb->setValue(angle);
		_layout->addWidget(dsb, 2, 0);

		horizontalLayout->addWidget(widget);
	}

	QuaternionProperty::~QuaternionProperty() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
