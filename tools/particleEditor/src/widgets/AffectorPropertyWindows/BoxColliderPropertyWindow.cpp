#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleAffectors/ParticleUniverseBoxCollider.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BoxColliderPropertyWindow::BoxColliderPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_COLLIDER_FRICTION = "Friction";
		PRNL_COLLIDER_BOUNCYNESS = "Bouncyness";
		PRNL_INTERSECTION_TYPE = "Intersection type";
		PRNL_COLLISION_TYPE = "Collision type";
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";
		PRNL_BOX_COLLIDER_WIDTH = "Box width";
		PRNL_BOX_COLLIDER_HEIGHT = "Box height";
		PRNL_BOX_COLLIDER_DEPTH = "Box depth";
		PRNL_BOX_COLLIDER_INNER = "Inner Collision";

		// Width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_WIDTH, PRNL_BOX_COLLIDER_WIDTH, ParticleUniverse::BoxCollider::DEFAULT_WIDTH));

		// Height: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_HEIGHT, PRNL_BOX_COLLIDER_HEIGHT, ParticleUniverse::BoxCollider::DEFAULT_HEIGHT));

		// Depth: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_BOX_COLLIDER_DEPTH, PRNL_BOX_COLLIDER_DEPTH, ParticleUniverse::BoxCollider::DEFAULT_DEPTH));

		// Inner Collision: bool
		append(new properties::BoolProperty(this, PRNL_BOX_COLLIDER_INNER, PRNL_BOX_COLLIDER_INNER, false));

		// Friction: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_FRICTION, PRNL_COLLIDER_FRICTION, ParticleUniverse::BoxCollider::DEFAULT_FRICTION));

		// Bouncyness: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_COLLIDER_BOUNCYNESS, PRNL_COLLIDER_BOUNCYNESS, ParticleUniverse::BoxCollider::DEFAULT_BOUNCYNESS));

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

	BoxColliderPropertyWindow::~BoxColliderPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
