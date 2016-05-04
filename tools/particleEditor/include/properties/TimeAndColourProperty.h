#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eVector4.h"

class QDoubleSpinBox;

namespace i6e {
namespace particleEditor {
namespace properties {

	class ColourWithAlphaProperty;

	class TimeAndColourProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TimeAndColourProperty(QWidget * par, QString label, QString name, const std::pair<double, Vec4> & value);

		/**
		 * \brief destructor
		 */
		~TimeAndColourProperty();

		void setTimeAndColour(const std::pair<double, Vec4> & value);

		std::pair<double, Vec4> getTimeAndColour() const {
			return _value;
		}

	private slots:
		void changedValue();

	private:
		QGridLayout * _layout;
		std::pair<double, Vec4> _value;
		ColourWithAlphaProperty * _colourProperty;
		QDoubleSpinBox * _doubleSpinBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_TIMEANDCOLOURPROPERTY_H__ */
