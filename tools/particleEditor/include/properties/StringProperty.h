#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_STRINGPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_STRINGPROPERTY_H__

#include "properties/Property.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class StringProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		StringProperty(QWidget * par, QString label, QString name, QString value);

		/**
		 * \brief destructor
		 */
		~StringProperty();

		void setString(QString value) override;

		QString getString() const override {
			return _value;
		}

	private slots:
		void stringChanged(QString value);

	protected:
		QString _value;
		QLineEdit * _lineEdit;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_STRINGPROPERTY_H__ */
