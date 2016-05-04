#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6e {
namespace particleEditor {
namespace widgets {
	class RendererPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class RendererPropertyWindowFactory {
	public:
		static widgets::RendererPropertyWindow * createRendererPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::RendererPropertyWindow * createRendererPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::RendererPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__ */
