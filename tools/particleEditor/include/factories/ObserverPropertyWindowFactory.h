#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6e {
namespace particleEditor {
namespace widgets {
	class ObserverPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class ObserverPropertyWindowFactory {
	public:
		static widgets::ObserverPropertyWindow * createObserverPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::ObserverPropertyWindow * createObserverPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::ObserverPropertyWindow * observerPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_OBSERVERPROPERTYWINDOWFACTORY_H__ */
