#include "widgets/ExternPropertyWindows/SphereColliderExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.h"

#include "ParticleAffectors/ParticleUniverseBaseCollider.h"
#include "Externs/ParticleUniverseSphereColliderExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	SphereColliderExternPropertyWindow::SphereColliderExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
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
		append(new properties::DoubleProperty(this, PRNL_EXTERN_THRESHOLD, PRNL_EXTERN_THRESHOLD, std::numeric_limits<float>::max()));

		// Friction: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BaseCollider::DEFAULT_FRICTION));

		// Bouncyness: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BaseCollider::DEFAULT_BOUNCYNESS));

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

		// Radius: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPHERE_COLLIDER_RADIUS, PRNL_SPHERE_COLLIDER_RADIUS, ParticleUniverse::SphereColliderExtern::DEFAULT_RADIUS));
	}

	SphereColliderExternPropertyWindow::~SphereColliderExternPropertyWindow() {
	}

	void SphereColliderExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		ExternPropertyWindow::copyAttributesFromExtern(externObject);

		// Copy properties from Extern to property window
		ParticleUniverse::SphereColliderExtern * sphereColliderExtern = static_cast<ParticleUniverse::SphereColliderExtern *>(externObject);

		// Distance Threshold: ParticleUniverse::Real
		setDouble(PRNL_EXTERN_THRESHOLD, sphereColliderExtern->getDistanceThreshold());

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_SPHERE_COLLIDER_RADIUS, sphereColliderExtern->getRadius());

		// Friction: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_FRICTION, sphereColliderExtern->getFriction());

		// Bouncyness: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_BOUNCYNESS, sphereColliderExtern->getBouncyness());

		// Intersection type: List
		ParticleUniverse::BaseCollider::IntersectionType intersectionType = sphereColliderExtern->getIntersectionType();
		QString intersectionTypeString = IST_POINT;
		if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX) {
			intersectionTypeString = IST_BOX;
		}
		setEnumString(PRNL_INTERSECTION_TYPE, intersectionTypeString);

		// Collision type: List
		ParticleUniverse::BaseCollider::CollisionType collisionType = sphereColliderExtern->getCollisionType();
		QString collisionTypeString = COLLT_NONE;
		if (collisionType == ParticleUniverse::BaseCollider::CT_BOUNCE) {
			collisionTypeString = COLLT_BOUNCE;
		} else if (collisionType == ParticleUniverse::BaseCollider::CT_FLOW) {
			collisionTypeString = COLLT_FLOW;
		}
		setEnumString(PRNL_COLLISION_TYPE, collisionTypeString);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
