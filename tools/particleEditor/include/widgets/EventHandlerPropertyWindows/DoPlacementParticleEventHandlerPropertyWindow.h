#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class DoPlacementParticleEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoPlacementParticleEventHandlerPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoPlacementParticleEventHandlerPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOPLACEMENTPARTICLEEVENTHANDLERPROPERTYWINDOW_H__ */
