#include "properties/Vec3Property.h"

#include <QDoubleSpinBox>
#include <QLabel>

namespace i6engine {
namespace particleEditor {
namespace properties {

	Vec3Property::Vec3Property(QWidget * par, QString label, QString name, ParticleUniverse::Vector3 value) : Property(par, label, name), _value(value) {
		QLabel * l = new QLabel("X", this);
		horizontalLayout->addWidget(l);
		QDoubleSpinBox * dsb = new QDoubleSpinBox(this);
		dsb->setMaximum(100000);
		dsb->setValue(value.x);
		horizontalLayout->addWidget(dsb);

		l = new QLabel("Y", this);
		horizontalLayout->addWidget(l);
		dsb = new QDoubleSpinBox(this);
		dsb->setMaximum(100000);
		dsb->setValue(value.y);
		horizontalLayout->addWidget(dsb);

		l = new QLabel("Z", this);
		horizontalLayout->addWidget(l);
		dsb = new QDoubleSpinBox(this);
		dsb->setMaximum(100000);
		dsb->setValue(value.z);
		horizontalLayout->addWidget(dsb);
	}

	Vec3Property::~Vec3Property() {
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */
