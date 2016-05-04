#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eQuaternion.h"

class QDoubleSpinBox;

namespace i6e {
namespace particleEditor {
namespace properties {

	class Vec3Property;

	class QuaternionProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		QuaternionProperty(QWidget * par, QString label, QString name, Quaternion value);

		/**
		 * \brief destructor
		 */
		~QuaternionProperty();

		void setQuaternion(Quaternion value) override;

		Quaternion getQuaternion() const override {
			return _value;
		}

	private slots:
		void changedValue();

	private:
		QGridLayout * _layout;
		Quaternion _value;
		Vec3Property * _vec3Property;
		QDoubleSpinBox * _doubleSpinBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__ */
