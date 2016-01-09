#ifndef __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__
#define __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__

#include "ui_widgetProperty.h"

namespace i6engine {
namespace particleEditor {
namespace properties {

	class Property : public QWidget, public Ui::propertyWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		Property(QWidget * par, QString label, QString name);

		/**
		 * \brief destructor
		 */
		virtual ~Property();

	protected:
		QString _label;
		QString _name;
	};

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_PROPERTIES_PROPERTY_H__ */
