#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"

#include "properties/BoolProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEventHandlers/ParticleUniverseDoAffectorEventHandler.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	DoAffectorEventHandlerPropertyWindow::DoAffectorEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EventHandlerPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_PRE_POST = QApplication::tr("Perform pre/post processing");

		// Affector component: Doesn't have a property, because this is established by means of a connection

		// Pre/Post Processing: bool
		append(new properties::BoolProperty(this, PRNL_PRE_POST, PRNL_PRE_POST, ParticleUniverse::DoAffectorEventHandler::DEFAULT_PRE_POST));
	}

	DoAffectorEventHandlerPropertyWindow::~DoAffectorEventHandlerPropertyWindow() {
	}

	void DoAffectorEventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		EventHandlerPropertyWindow::copyAttributesFromEventHandler(eventHandler);

		// Copy properties from eventHandler to property window
		ParticleUniverse::DoAffectorEventHandler * doAffectorEventHandler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(eventHandler);

		// Pre/Post Processing: bool
		setBool(PRNL_PRE_POST, doAffectorEventHandler->getPrePost());
	}

	void DoAffectorEventHandlerPropertyWindow::copyAttributeToEventHandler(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::DoAffectorEventHandler * handler = static_cast<ParticleUniverse::DoAffectorEventHandler *>(_owner->getPUElement());
		if (!handler) {
			return;
		}

		if (propertyName == PRNL_PRE_POST) {
			// Pre/Post Processing: bool
			handler->setPrePost(prop->getBool());
		} else {
			// Update handler with another attribute
			EventHandlerPropertyWindow::copyAttributeToEventHandler(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
