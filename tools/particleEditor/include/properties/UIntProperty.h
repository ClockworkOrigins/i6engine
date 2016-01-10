#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__

#include "properties/Property.h"

class QSpinBox;

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

		void setUInt(unsigned int value) override;

	private slots:
		void changedValue();

	private:
		unsigned int _value;
		QSpinBox * _spinBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_UINTPROPERTY_H__ */
