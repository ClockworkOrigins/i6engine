#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eQuaternion.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

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

	protected:
		QGridLayout * _layout;
		Quaternion _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_QUATERNIONPROPERTY_H__ */
