#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_BEHAVIOUR_TYPE = "";

	class BehaviourPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BehaviourPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~BehaviourPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__ */
