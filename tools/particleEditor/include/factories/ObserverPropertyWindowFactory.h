#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class ObserverPropertyWindow;
} /* namespace widgets */
namespace factories {

	class ObserverPropertyWindowFactory {
	public:
		static widgets::ObserverPropertyWindow * createObserverPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::ObserverPropertyWindow * createObserverPropertyWindow(QString subType, widgets::ObserverPropertyWindow * observerPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__ */
