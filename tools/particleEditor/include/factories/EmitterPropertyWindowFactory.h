#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_EMITTERPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_EMITTERPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class EmitterPropertyWindow;
} /* namespace widgets */
namespace factories {

	class EmitterPropertyWindowFactory {
	public:
		static widgets::EmitterPropertyWindow * createEmitterPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::EmitterPropertyWindow * createEmitterPropertyWindow(QString subType, widgets::EmitterPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_EMITTERPROPERTYWINDOWFACTORY_H__ */
