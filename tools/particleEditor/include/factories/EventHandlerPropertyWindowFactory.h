#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_EVENTHANDLERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_EVENTHANDLERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class EventHandlerPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class EventHandlerPropertyWindowFactory {
	public:
		static widgets::EventHandlerPropertyWindow * createEventHandlerPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::EventHandlerPropertyWindow * createEventHandlerPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::EventHandlerPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_EVENTHANDLERPROPERTYWINDOWFACTORY_H__ */
