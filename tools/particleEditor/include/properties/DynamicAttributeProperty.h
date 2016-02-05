#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__

#include "properties/Property.h"

class QComboBox;
class QDoubleSpinBox;

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

		void setDynamicAttribute(ParticleUniverse::DynamicAttribute * value) override;

		ParticleUniverse::DynamicAttribute * getDynamicAttribute() const override;

	private slots:
		void changedDynamicType();
		void changedValue();

	private:
		enum PropertyTypes {
			Value,
			MinValue,
			MaxValue
		};
		QWidget * _widget;
		QGridLayout * _layout;
		ParticleUniverse::DynamicAttribute * _value;
		std::vector<QWidget *> _widgets;
		QComboBox * _comboBox;
		std::map<PropertyTypes, QDoubleSpinBox *> _values;

		void createGUI();
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__ */
