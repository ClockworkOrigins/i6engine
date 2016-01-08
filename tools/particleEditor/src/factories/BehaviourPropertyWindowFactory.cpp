#include "factories/BehaviourPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/BehaviourPropertyWindows/SlaveBehaviourPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Behaviour Property Window
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(parent, name);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(parent, name);
		} else {
			return new widgets::BehaviourPropertyWindow(parent, name);
		}
		return nullptr;
	}

	/**
	Create a specific Behaviour Property Window and propagate the attributes from the BehaviourPropertyWindow
	*/
	widgets::BehaviourPropertyWindow * BehaviourPropertyWindowFactory::createBehaviourPropertyWindow(QString subType, widgets::BehaviourPropertyWindow * behaviourPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow);
		} else if (subType == widgets::CST_BEHAVIOUR_SLAVE) {
			return new widgets::SlaveBehaviourPropertyWindow(behaviourPropertyWindow);
		} else {
			return new widgets::BehaviourPropertyWindow(behaviourPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
