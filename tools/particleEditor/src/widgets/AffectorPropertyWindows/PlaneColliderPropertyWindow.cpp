#include "widgets/AffectorPropertyWindows/PlaneColliderPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	PlaneColliderPropertyWindow::PlaneColliderPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_PLANE_COLLIDER_NORMAL = "Plane normal";
		PRNL_COLLIDER_FRICTION = "Friction";
		PRNL_COLLIDER_BOUNCYNESS = "Bouncyness";
		PRNL_INTERSECTION_TYPE = "Intersection type";
		PRNL_COLLISION_TYPE = "Collision type";
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";

		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

		// Normal: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_PLANE_COLLIDER_NORMAL, PRNL_PLANE_COLLIDER_NORMAL, ParticleUniverse::PlaneCollider::DEFAULT_NORMAL));

		// Friction: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::PlaneCollider::DEFAULT_FRICTION));

		// Bouncyness: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::PlaneCollider::DEFAULT_BOUNCYNESS));

		// Intersection type: List
		QStringList intersectionType;
		intersectionType.append(IST_POINT);
		intersectionType.append(IST_BOX);
		append(new properties::EnumProperty(this, PRNL_INTERSECTION_TYPE, PRNL_INTERSECTION_TYPE, intersectionType));

		// Collision type: List
		QStringList collisionTypes;
		collisionTypes.append(COLLT_NONE);
		collisionTypes.append(COLLT_BOUNCE);
		collisionTypes.append(COLLT_FLOW);
		append(new properties::EnumProperty(this, PRNL_COLLISION_TYPE, PRNL_COLLISION_TYPE, collisionTypes));
	}

	PlaneColliderPropertyWindow::~PlaneColliderPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
