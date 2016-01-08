#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class BehaviourPropertyWindow;
} /* namespace widgets */
namespace factories {

	class BehaviourPropertyWindowFactory {
	public:
		static widgets::BehaviourPropertyWindow * createBehaviourPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::BehaviourPropertyWindow * createBehaviourPropertyWindow(QString subType, widgets::BehaviourPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_BEHAVIOURPROPERTYWINDOWFACTORY_H__ */
