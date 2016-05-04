#include "properties/Vec3Property.h"

#include <QDoubleSpinBox>
#include <QLabel>

namespace i6e {
namespace particleEditor {
namespace properties {

	Vec3Property::Vec3Property(QWidget * par, QString label, QString name, ParticleUniverse::Vector3 value) : Property(par, label, name), _value(value), _doubleSpinBoxX(nullptr), _doubleSpinBoxY(nullptr), _doubleSpinBoxZ(nullptr), _set(false) {
		QLabel * l = new QLabel("X", this);
		horizontalLayout->addWidget(l);
		_doubleSpinBoxX = new QDoubleSpinBox(this);
		_doubleSpinBoxX->setMinimum(-999999);
		_doubleSpinBoxX->setMaximum(999999);
		_doubleSpinBoxX->setValue(value.x);
		horizontalLayout->addWidget(_doubleSpinBoxX);
		_doubleSpinBoxX->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
		connect(_doubleSpinBoxX, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));

		l = new QLabel("Y", this);
		horizontalLayout->addWidget(l);
		_doubleSpinBoxY = new QDoubleSpinBox(this);
		_doubleSpinBoxY->setMinimum(-999999);
		_doubleSpinBoxY->setMaximum(999999);
		_doubleSpinBoxY->setValue(value.y);
		horizontalLayout->addWidget(_doubleSpinBoxY);
		_doubleSpinBoxY->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
		connect(_doubleSpinBoxY, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));

		l = new QLabel("Z", this);
		horizontalLayout->addWidget(l);
		_doubleSpinBoxZ = new QDoubleSpinBox(this);
		_doubleSpinBoxZ->setMinimum(-999999);
		_doubleSpinBoxZ->setMaximum(999999);
		_doubleSpinBoxZ->setValue(value.z);
		horizontalLayout->addWidget(_doubleSpinBoxZ);
		_doubleSpinBoxZ->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Fixed);
		connect(_doubleSpinBoxZ, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
	}

	Vec3Property::~Vec3Property() {
	}

	void Vec3Property::setVector3(ParticleUniverse::Vector3 value) {
		// set is used to know in valueChanged when value is not edited via GUI but set from code to avoid notification
		_set = true;
		_value = value;
		_doubleSpinBoxX->setValue(value.x);
		_doubleSpinBoxY->setValue(value.y);
		_doubleSpinBoxZ->setValue(value.z);
		_set = false;
	}

	void Vec3Property::changedValue() {
		if (!_set) {
			_value.x = _doubleSpinBoxX->value();
			_value.y = _doubleSpinBoxY->value();
			_value.z = _doubleSpinBoxZ->value();
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
