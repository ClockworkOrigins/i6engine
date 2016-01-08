#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class ExternPropertyWindow;
} /* namespace widgets */
namespace factories {

	class ExternPropertyWindowFactory {
	public:
		static widgets::ExternPropertyWindow * createExternPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::ExternPropertyWindow * createExternPropertyWindow(QString subType, widgets::ExternPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_EXTERNPROPERTYWINDOWFACTORY_H__ */
