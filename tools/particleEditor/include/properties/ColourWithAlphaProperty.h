#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__

#include "properties/Property.h"

#include "i6engine/math/i6eVector4.h"

class QSpinBox;

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

		void setColourWithAlpha(Vec4 value) override;

		Vec4 getColourWithAlpha() const override {
			return _value;
		}

	private slots:
		void valueChanged();

	private:
		QGridLayout * _layout;
		Vec4 _value;
		QSpinBox * _spinBoxR;
		QSpinBox * _spinBoxG;
		QSpinBox * _spinBoxB;
		QSpinBox * _spinBoxA;
		bool _set;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_COLOURWITHALPHAPROPERTY_H__ */
