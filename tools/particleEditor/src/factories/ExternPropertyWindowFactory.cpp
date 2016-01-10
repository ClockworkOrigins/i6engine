#include "factories/ExternPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/GravityExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/SphereColliderExternPropertyWindow.h"
#include "widgets/ExternPropertyWindows/VortexExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Extern Property Window
	*/
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QWidget * parent, widgets::WidgetEditComponent * owner, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(parent, owner, name);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(parent, owner, name);
		} else {
			return new widgets::ExternPropertyWindow(parent, owner, name);
		}
		return nullptr;
	}

	/**
	Create a specific Extern Property Window and propagate the attributes from the ExternPropertyWindow
	*/
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QString subType, widgets::WidgetEditComponent * owner, widgets::ExternPropertyWindow * externPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(externPropertyWindow, owner);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(externPropertyWindow, owner);
		} else {
			return new widgets::ExternPropertyWindow(externPropertyWindow, owner);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
