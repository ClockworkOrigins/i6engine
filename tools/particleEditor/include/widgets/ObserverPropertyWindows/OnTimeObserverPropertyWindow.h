#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONTIMEOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONTIMEOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class OnTimeObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnTimeObserverPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnTimeObserverPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONTIMEOBSERVERPROPERTYWINDOW_H__ */
