#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ENABLE_COMPONENT_TRUE_FALSE = "";

	class DoEnableComponentEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoEnableComponentEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoEnableComponentEventHandlerPropertyWindow();

		void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) override;

	private:
		void copyAttributeToEventHandler(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOENABLECOMPONENTEVENTHANDLERPROPERTYWINDOW_H__ */
