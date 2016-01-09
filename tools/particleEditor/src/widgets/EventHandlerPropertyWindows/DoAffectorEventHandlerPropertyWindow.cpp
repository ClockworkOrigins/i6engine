#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(QWidget * par, QString name) : EventHandlerPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_PRE_POST = "Perform pre/post processing";

		// Affector component: Doesn't have a property, because this is established by means of a connection

		// Pre/Post Processing: bool
		append(new properties::BoolProperty(this, PRNL_PRE_POST, PRNL_PRE_POST, ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST));
	}

	DoAffectorEventHandlerPropertyWindow::~DoAffectorEventHandlerPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
