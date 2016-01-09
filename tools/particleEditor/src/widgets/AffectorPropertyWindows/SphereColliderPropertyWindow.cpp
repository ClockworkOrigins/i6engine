#include "widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleAffectors/ParticleUniverseSphereCollider.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SphereColliderPropertyWindow::SphereColliderPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_SPHERE_COLLIDER_RADIUS = "Sphere radius";
		PRNL_SPHERE_COLLIDER_INNER = "Inner Collision";
		PRNL_COLLIDER_FRICTION = "Friction";
		PRNL_COLLIDER_BOUNCYNESS = "Bouncyness";
		PRNL_INTERSECTION_TYPE = "Intersection type";
		PRNL_COLLISION_TYPE = "Collision type";
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPHERE_COLLIDER_RADIUS, PRNL_SPHERE_COLLIDER_RADIUS, ParticleUniverse::SphereCollider::DEFAULT_RADIUS));

		// Inner Collision: bool
		append(new properties::BoolProperty(this, PRNL_SPHERE_COLLIDER_INNER, PRNL_SPHERE_COLLIDER_INNER, false));

		// Friction: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::SphereCollider::DEFAULT_FRICTION));

		// Bouncyness: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::SphereCollider::DEFAULT_BOUNCYNESS));

		// Intersection type: List
		QStringList intersectionTypes;
		intersectionTypes.append(IST_POINT);
		intersectionTypes.append(IST_BOX);
		append(new properties::EnumProperty(this, PRNL_INTERSECTION_TYPE, PRNL_INTERSECTION_TYPE, intersectionTypes));

		// Collision type: List
		QStringList collisionTypes;
		collisionTypes.append(COLLT_NONE);
		collisionTypes.append(COLLT_BOUNCE);
		collisionTypes.append(COLLT_FLOW);
		append(new properties::EnumProperty(this, PRNL_COLLISION_TYPE, PRNL_COLLISION_TYPE, collisionTypes));
	}

	SphereColliderPropertyWindow::~SphereColliderPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
