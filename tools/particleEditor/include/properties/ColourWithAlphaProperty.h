#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eVector4.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class ColourWithAlphaProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ColourWithAlphaProperty(QWidget * par, QString label, QString name, Vec4 value);

		/**
		 * \brief destructor
		 */
		~ColourWithAlphaProperty();

	protected:
		QGridLayout * _layout;
		Vec4 _value;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__ */
