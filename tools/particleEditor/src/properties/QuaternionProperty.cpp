#include "properties/QuaternionProperty.h"

#include "i6engine/math/i6eVector.h"

#include "properties/Vec3Property.h"

#include "widgets/PropertyWindow.h"

#include <QLabel>
#include <QDoubleSpinBox>

namespace i6e {
namespace particleEditor {
namespace properties {

	QuaternionProperty::QuaternionProperty(QWidget * par, QString label, QString name, Quaternion value) : Property(par, label, name), _layout(nullptr), _value(value), _vec3Property(nullptr), _doubleSpinBox(nullptr) {
		QWidget * widget = new QWidget(this);
		Vec3 axis;
		double angle;
		value.toAxisAngle(axis, angle);

		_layout = new QGridLayout(widget);
		widget->setLayout(_layout);
		_vec3Property = new Vec3Property(widget, QApplication::tr("Axis"), QApplication::tr("Axis"), axis.toOgre());
		_layout->addWidget(_vec3Property, 0, 0);
		connect(_vec3Property, SIGNAL(changed(QString)), this, SLOT(changedValue()));
		_vec3Property->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);

		QLabel * l = new QLabel(QApplication::tr("Angle"), widget);
		_layout->addWidget(l, 1, 0);
		_doubleSpinBox = new QDoubleSpinBox(this);
		_doubleSpinBox->setMinimum(-360.0);
		_doubleSpinBox->setMaximum(360.0);
		_doubleSpinBox->setValue(angle);
		_layout->addWidget(_doubleSpinBox, 2, 0);
		connect(_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changedValue()));
		l->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
		_doubleSpinBox->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);

		horizontalLayout->addWidget(widget);
	}

	QuaternionProperty::~QuaternionProperty() {
	}

	void QuaternionProperty::setQuaternion(Quaternion value) {
		_value = value;
		Vec3 axis;
		double angle;
		value.toAxisAngle(axis, angle);
		_vec3Property->setVector3(axis.toOgre());
		_doubleSpinBox->setValue(angle);
	}

	void QuaternionProperty::changedValue() {
		Quaternion q = Quaternion(Vec3(_vec3Property->getVector3().normalisedCopy()), _doubleSpinBox->value());
		if (_value != q) {
			_value = Quaternion(Vec3(_vec3Property->getVector3().normalisedCopy()), _doubleSpinBox->value());
			triggerChangedSignal();
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
