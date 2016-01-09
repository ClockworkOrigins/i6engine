#include "widgets/EventHandlerPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseEventHandler.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	EventHandlerPropertyWindow::EventHandlerPropertyWindow(QWidget * par, QString name) : PropertyWindow(par, name) {
		// Set the (internationalized) property names
		CST_HANDLER_DO_AFFECTOR = HANDLER_DO_AFFECTOR;
		CST_HANDLER_DO_ENABLE_COMPONENT = HANDLER_DO_ENABLE_COMPONENT;
		CST_HANDLER_DO_EXPIRE = HANDLER_DO_EXPIRE;
		CST_HANDLER_DO_FREEZE = HANDLER_DO_FREEZE;
		CST_HANDLER_DO_PLACEMENT_PARTICLE = HANDLER_DO_PLACEMENT_PARTICLE;
		CST_HANDLER_DO_SCALE = HANDLER_DO_SCALE;
		CST_HANDLER_DO_STOP_SYSTEM = HANDLER_DO_STOP_SYSTEM;
		PRNL_HANDLER_TYPE = "Event handler type";
		PRNL_NAME = "Name";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create a handler)
		QStringList types;
		types.append(CST_HANDLER_DO_AFFECTOR);
		types.append(CST_HANDLER_DO_ENABLE_COMPONENT);
		types.append(CST_HANDLER_DO_EXPIRE);
		types.append(CST_HANDLER_DO_FREEZE);
		types.append(CST_HANDLER_DO_PLACEMENT_PARTICLE);
		types.append(CST_HANDLER_DO_SCALE);
		types.append(CST_HANDLER_DO_STOP_SYSTEM);
		append(new properties::EnumProperty(this, PRNL_HANDLER_TYPE, PRNL_HANDLER_TYPE, types));
	}

	EventHandlerPropertyWindow::~EventHandlerPropertyWindow() {
	}

	void EventHandlerPropertyWindow::copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(eventHandler->getName()));

		// Type: List of types
		setEnumString(PRNL_HANDLER_TYPE, QString::fromStdString(eventHandler->getEventHandlerType()));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
