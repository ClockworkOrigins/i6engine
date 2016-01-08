#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class ColourAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ColourAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~ColourAffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__ */
