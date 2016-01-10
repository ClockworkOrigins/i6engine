#include "widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"

#include "Externs/ParticleUniverseBoxColliderExtern.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
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

	void BoxColliderExternPropertyWindow::copyAttributesFromExtern(ParticleUniverse::Extern * externObject) {
		ExternPropertyWindow::copyAttributesFromExtern(externObject);

		// Copy properties from Extern to property window
		ParticleUniverse::BoxColliderExtern * boxColliderExtern = static_cast<ParticleUniverse::BoxColliderExtern *>(externObject);

		// Distance Threshold: ParticleUniverse::Real
		setDouble(PRNL_EXTERN_THRESHOLD, boxColliderExtern->getDistanceThreshold());

		// Width: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_WIDTH, boxColliderExtern->getWidth());

		// Height: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_HEIGHT, boxColliderExtern->getHeight());

		// Depth: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_DEPTH, boxColliderExtern->getDepth());

		// Friction: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_FRICTION, boxColliderExtern->getFriction());

		// Bouncyness: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_BOUNCYNESS, boxColliderExtern->getBouncyness());

		// Intersection type: List
		ParticleUniverse::BaseCollider::IntersectionType intersectionType = boxColliderExtern->getIntersectionType();
		QString intersectionTypeString = IST_POINT;
		if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX) {
			intersectionTypeString = IST_BOX;
		}
		setEnumString(PRNL_INTERSECTION_TYPE, intersectionTypeString);

		// Collision type: List
		ParticleUniverse::BaseCollider::CollisionType collisionType = boxColliderExtern->getCollisionType();
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
