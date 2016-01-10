#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class ExternPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class ExternPropertyWindowFactory {
	public:
		static widgets::ExternPropertyWindow * createExternPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::ExternPropertyWindow * createExternPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::ExternPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__ */
