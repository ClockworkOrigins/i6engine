#include "factories/EventHandlerPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/EventHandlerPropertyWindows/DoAffectorEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoEnableComponentEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoExpireEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoFreezeEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoPlacementParticleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoScaleEventHandlerPropertyWindow.h"
#include "widgets/EventHandlerPropertyWindows/DoStopSystemEventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific EventHandler Property Window
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(parent, name);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(parent, name);
		} else {
			return new widgets::EventHandlerPropertyWindow(parent, name);
		}
		return nullptr;
	}

	/**
	Create a specific EventHandler Property Window and propagate the attributes from the EventHandlerPropertyWindow
	*/
	widgets::EventHandlerPropertyWindow * EventHandlerPropertyWindowFactory::createEventHandlerPropertyWindow(QString subType, widgets::EventHandlerPropertyWindow * eventHandlerPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_AFFECTOR) {
			return new widgets::DoAffectorEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_ENABLE_COMPONENT) {
			return new widgets::DoEnableComponentEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_EXPIRE) {
			return new widgets::DoExpireEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_FREEZE) {
			return new widgets::DoFreezeEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_PLACEMENT_PARTICLE) {
			return new widgets::DoPlacementParticleEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_SCALE) {
			return new widgets::DoScaleEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else if (subType == widgets::CST_HANDLER_DO_STOP_SYSTEM) {
			return new widgets::DoStopSystemEventHandlerPropertyWindow(eventHandlerPropertyWindow);
		} else {
			return new widgets::EventHandlerPropertyWindow(eventHandlerPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
