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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
