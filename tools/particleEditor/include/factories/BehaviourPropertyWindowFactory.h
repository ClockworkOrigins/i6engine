#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6e {
namespace particleEditor {
namespace widgets {
	class BehaviourPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class BehaviourPropertyWindowFactory {
	public:
		static widgets::BehaviourPropertyWindow * createBehaviourPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::BehaviourPropertyWindow * createBehaviourPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::BehaviourPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__ */
