#ifndef __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__
#define __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__

#include <QWidget>

namespace i6e {
namespace particleEditor {
namespace widgets {
	class AffectorPropertyWindow;
	class WidgetEditComponent;
} /* namespace widgets */
namespace factories {

	class AffectorPropertyWindowFactory {
	public:
		static widgets::AffectorPropertyWindow * createAffectorPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType);
		static widgets::AffectorPropertyWindow * createAffectorPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::AffectorPropertyWindow * rendererPropertyWindow);
	};

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_FACTORIES_AFFECTORPROPERTYWINDOWFACTORY_H__ */
