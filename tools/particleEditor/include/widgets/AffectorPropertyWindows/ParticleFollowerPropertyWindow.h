#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_PARTICLEFOLLOWERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_PARTICLEFOLLOWERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class ParticleFollowerPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ParticleFollowerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~ParticleFollowerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_PARTICLEFOLLOWERPROPERTYWINDOW_H__ */
