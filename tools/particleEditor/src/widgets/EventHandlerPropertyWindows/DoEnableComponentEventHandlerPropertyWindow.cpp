#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EventHandlerPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ENABLE_COMPONENT_TRUE_FALSE = QApplication::tr("Enable component");

		// Component to enable/disable: Realised by means of a connection

		// Enable/Disable: bool
		append(new properties::BoolProperty(this, PRNL_ENABLE_COMPONENT_TRUE_FALSE, PRNL_ENABLE_COMPONENT_TRUE_FALSE, true));
	}

	DoEnableComponentEventHandlerPropertyWindow::~DoEnableComponentEventHandlerPropertyWindow() {
	}

	void DoEnableComponentEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoEnableComponentEventHandler * doEnableComponentEventHandler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(eventHandler);

		// Enable/Disable: bool
		setBool(PRNL_ENABLE_COMPONENT_TRUE_FALSE, doEnableComponentEventHandler->isComponentEnabled());
	}

	void DoEnableComponentEventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::DoEnableComponentEventHandler * handler = static_cast<ParticleUniverse::DoEnableComponentEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_ENABLE_COMPONENT_TRUE_FALSE) {
			// Enable/Disable: bool
			handler->setComponentEnabled(prop->getBool());
		} else {
			// Update handler with another attribute
			EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
