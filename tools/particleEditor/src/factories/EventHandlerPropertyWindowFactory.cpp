#include "factories/EventHandlerPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoExpireEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoFreezeEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoStopSystemEventHandlerPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace factories {

	/**
	Create a specific EventHandler Property Window
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(parent, owner, name);
		} else {
			return new widgets::EventHandlerPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific EventHandler Property Window and propagate the attributes from the EventHandlerPropertyWindow
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::EventHandlerPropertyWindow * eventHandlerPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		} else {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */
