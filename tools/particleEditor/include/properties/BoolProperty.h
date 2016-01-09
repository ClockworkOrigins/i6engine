#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_BOOLPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_BOOLPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class BoolProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BoolProperty(QWidget * par, QString label, QString name, bool value);

		/**
		 * \brief destructor
		 */
		~BoolProperty();

	protected:
		bool _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_BOOLPROPERTY_H__ */
