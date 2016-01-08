#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class AffectorPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		AffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~AffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__ */
