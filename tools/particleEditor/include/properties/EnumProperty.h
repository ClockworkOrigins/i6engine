#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_ENUMPROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_ENUMPROPERTY_H__

#include "properties/Property.h"

class QComboBox;

namespace i6engine {
namespace particleEditor {
namespace properties {

	class EnumProperty : public Property {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EnumProperty(QWidget * par, QString label, QString name, QStringList value);

		/**
		 * \brief destructor
		 */
		~EnumProperty();

		void setCurrentIndex(int index);

		void setEnumString(QString value) override;

		int getCurrentIndex() const override;

	private slots:
		void changedValue();

	private:
		QStringList _value;
		QComboBox * _comboBox;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_ENUMPROPERTY_H__ */
