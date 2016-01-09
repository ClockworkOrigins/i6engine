#include "widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"

#include "Externs/ParticleUniverseBoxColliderExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(QWidget * par, QString name) : ExternPropertyWindow(par, name) {
		// Set the (internationalized) property names
		// TODO
		PRNL_COLLIDER_FRICTION = "Friction";
		PRNL_COLLIDER_BOUNCYNESS = "Bouncyness";
		PRNL_INTERSECTION_TYPE = "Intersection type";
		PRNL_COLLISION_TYPE = "Collision type";
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";
		PRNL_EXTERN_THRESHOLD = "Distance threshold";

		// Distance Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<double>::max()));

		// Friction: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BoxCollider::DEFAULT_FRICTION));

		// Bouncyness: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BoxCollider::DEFAULT_BOUNCYNESS));

		// Intersection Type: List
		QStringList intersectionTypes;
		intersectionTypes.append(IST_POINT);
		intersectionTypes.append(IST_BOX);
		append(new properties::EnumProperty(this, PRNL_INTERSECTION_TYPE, PRNL_INTERSECTION_TYPE, intersectionTypes));

		// Collision Type: List
		QStringList collisionTypes;
		collisionTypes.append(COLLT_NONE);
		collisionTypes.append(COLLT_BOUNCE);
		collisionTypes.append(COLLT_FLOW);
		append(new properties::EnumProperty(this, PRNL_COLLISION_TYPE, PRNL_COLLISION_TYPE, collisionTypes));

		// Box Width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_WIDTH, PRNL_BOX_COLLIDER_WIDTH, ParticleUniverse::BoxCollider::DEFAULT_WIDTH));

		// Box Height: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_HEIGHT, PRNL_BOX_COLLIDER_HEIGHT, ParticleUniverse::BoxCollider::DEFAULT_HEIGHT));

		// Box Depth: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_DEPTH, PRNL_BOX_COLLIDER_DEPTH, ParticleUniverse::BoxCollider::DEFAULT_DEPTH));
	}

	BoxColliderExternPropertyWindow::~BoxColliderExternPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
