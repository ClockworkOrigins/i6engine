#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__

#include "properties/Property.h"

namespace ParticleUniverse {
	class DynamicAttribute;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace properties {

	class DynamicAttributeProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DynamicAttributeProperty(QWidget * par, QString label, QString name, ParticleUniverse::DynamicAttribute * value);

		/**
		 * \brief destructor
		 */
		~DynamicAttributeProperty();

	protected:
		QGridLayout * _layout;
		ParticleUniverse::DynamicAttribute * _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__ */
