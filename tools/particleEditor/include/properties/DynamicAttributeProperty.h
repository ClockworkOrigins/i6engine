#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__

#include "properties/Property.h"

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

	protected:
		QWidget * _widget;
		QGridLayout * _layout;
		ParticleUniverse::DynamicAttribute * _value;
		std::vector<QWidget *> _widgets;

	private:
		void createGUI();
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DYNAMICATTRIBUTEPROPERTY_H__ */
