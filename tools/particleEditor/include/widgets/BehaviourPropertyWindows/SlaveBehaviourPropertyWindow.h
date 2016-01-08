#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SLAVEBEHAVIOURPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SLAVEBEHAVIOURPROPERTYWINDOW_H__

#include "widgets/BehaviourPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class SlaveBehaviourPropertyWindow : public BehaviourPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SlaveBehaviourPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~SlaveBehaviourPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SLAVEBEHAVIOURPROPERTYWINDOW_H__ */
