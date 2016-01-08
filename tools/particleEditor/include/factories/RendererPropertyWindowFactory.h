#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class RendererPropertyWindow;
} /* namespace widgets */
namespace factories {

	class RendererPropertyWindowFactory {
	public:
		static widgets::RendererPropertyWindow * createRendererPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::RendererPropertyWindow * createRendererPropertyWindow(QString subType, widgets::RendererPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_RENDERERPROPERTYWINDOWFACTORY_H__ */
