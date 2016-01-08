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
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(parent, name);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(parent, name);
		} else {
			return new widgets::ExternPropertyWindow(parent, name);
		}
		return nullptr;
	}

	/**
	Create a specific Extern Property Window and propagate the attributes from the ExternPropertyWindow
	*/
	widgets::ExternPropertyWindow * ExternPropertyWindowFactory::createExternPropertyWindow(QString subType, widgets::ExternPropertyWindow * externPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ExternPropertyWindow(externPropertyWindow);
		} else if (subType == widgets::CST_EXTERN_BOX_COLLIDER) {
			return new widgets::BoxColliderExternPropertyWindow(externPropertyWindow);
		} else if (subType == widgets::CST_EXTERN_GRAVITY) {
			return new widgets::GravityExternPropertyWindow(externPropertyWindow);
		} else if (subType == widgets::CST_EXTERN_SPHERE_COLLIDER) {
			return new widgets::SphereColliderExternPropertyWindow(externPropertyWindow);
		} else if (subType == widgets::CST_EXTERN_VORTEX) {
			return new widgets::VortexExternPropertyWindow(externPropertyWindow);
		} else {
			return new widgets::ExternPropertyWindow(externPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
