#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6engine {
namespace particleEditor {
namespace widgets {
	class AffectorPropertyWindow;
} /* namespace widgets */
namespace factories {

	class AffectorPropertyWindowFactory {
	public:
		static widgets::AffectorPropertyWindow * createAffectorPropertyWindow(QWidget * parent, const QString & name, QString subType);
		static widgets::AffectorPropertyWindow * createAffectorPropertyWindow(QString subType, widgets::AffectorPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__ */
