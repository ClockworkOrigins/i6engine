#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__

#include "properties/Property.h"

class QDoubleSpinBox;

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

		void setDouble(double value) override;

		double getDouble() const override {
			return _value;
		}

	private slots:
		void valueChanged();

	protected:
		double _value;
		QDoubleSpinBox * _doubleSpinBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_DOUBLEPROPERTY_H__ */
