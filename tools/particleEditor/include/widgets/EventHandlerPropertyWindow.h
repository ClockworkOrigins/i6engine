#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleEventHandler;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_HANDLER_TYPE = "";

	class EventHandlerPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~EventHandlerPropertyWindow();

		virtual void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__ */
