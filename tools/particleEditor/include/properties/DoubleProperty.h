#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class DoubleProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoubleProperty(QWidget * par, QString label, QString name, double value);

		/**
		 * \brief destructor
		 */
		~DoubleProperty();

	protected:
		double _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__ */
