#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "ParticleEventHandlers/ParticleUniverseDoEnableComponentEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	DoEnableComponentEventHandlerPropertyWindow::DoEnableComponentEventHandlerPropertyWindow(QWidget * par, QString name) : EventHandlerPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ENABLE_COMPONENT_TRUE_FALSE = "Enable component";

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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
