#include "factories/BehaviourPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/BehaviourPropertyWindows/SlaveBehaviourPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Behaviour Property Window
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(parent, owner, name);
		} else {
			return new widgets::BehaviourPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Behaviour Property Window and propagate the attributes from the BehaviourPropertyWindow
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::BehaviourPropertyWindow * behaviourPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow, owner);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(behaviourPropertyWindow, owner);
		} else {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
