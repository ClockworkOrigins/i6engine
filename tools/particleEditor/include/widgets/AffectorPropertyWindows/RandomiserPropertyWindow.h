#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RANDOMISERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RANDOMISERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class RandomiserPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RandomiserPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~RandomiserPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RANDOMISERPROPERTYWINDOW_H__ */
