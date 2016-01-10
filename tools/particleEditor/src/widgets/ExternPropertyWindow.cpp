#include "widgets/ExternPropertyWindow.h"

#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	ExternPropertyWindow::ExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : PropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		CST_EXTERN_BOX_COLLIDER = EXTERN_BOX_COLLIDER;
		CST_EXTERN_SPHERE_COLLIDER = EXTERN_SPHERE_COLLIDER;
		CST_EXTERN_GRAVITY = EXTERN_GRAVITY;
		CST_EXTERN_PHYSX_ACTOR = EXTERN_PHYSX_ACTOR;
		CST_EXTERN_PHYSX_FLUID = EXTERN_PHYSX_FLUID;
		CST_EXTERN_VORTEX = EXTERN_VORTEX;
		PRNL_EXTERN_TYPE = "Extern type";
		PRNL_NAME = "Name";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create an extern)
		QStringList types;
		types.append(CST_EXTERN_BOX_COLLIDER);
		types.append(CST_EXTERN_GRAVITY);
		types.append(CST_EXTERN_SPHERE_COLLIDER);
		types.append(CST_EXTERN_VORTEX);
		append(new properties::EnumProperty(this, PRNL_EXTERN_TYPE, PRNL_EXTERN_TYPE, types));
	}

	ExternPropertyWindow::~ExternPropertyWindow() {
	}

	void ExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		// Name: Ogre::String
		setString(PRNL_NAME, QString::fromStdString(externObject->getName()));

		// Type: List of types
		setEnumString(PRNL_EXTERN_TYPE, QString::fromStdString(externObject->getExternType()));
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
