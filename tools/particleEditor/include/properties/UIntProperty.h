#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class UIntProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		UIntProperty(QWidget * par, QString label, QString name, unsigned int value);

		/**
		 * \brief destructor
		 */
		~UIntProperty();

	protected:
		unsigned int _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__ */
