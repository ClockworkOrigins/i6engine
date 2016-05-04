#include "widgets/AffectorPropertyWindows/SphereColliderPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseSphereCollider.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	SphereColliderPropertyWindow::SphereColliderPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_SPHERE_COLLIDER_RADIUS = QApplication::tr("Sphere radius");
		PRNL_SPHERE_COLLIDER_INNER = QApplication::tr("Inner Collision");
		PRNL_COLLIDER_FRICTION = QApplication::tr("Friction");
		PRNL_COLLIDER_BOUNCYNESS = QApplication::tr("Bouncyness");
		PRNL_INTERSECTION_TYPE = QApplication::tr("Intersection type");
		PRNL_COLLISION_TYPE = QApplication::tr("Collision type");
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

	void SphereColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::SphereCollider * sphereCollider = static_cast<ParticleUniverse::SphereCollider *>(affector);

		// Radius: ParticleUniverse::Real
		setDouble(PRNL_SPHERE_COLLIDER_RADIUS, sphereCollider->getRadius());

		// Inner Collision: bool
		setBool(PRNL_SPHERE_COLLIDER_INNER, sphereCollider->isInnerCollision());

		// Friction: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_FRICTION, sphereCollider->getFriction());

		// Bouncyness: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_BOUNCYNESS, sphereCollider->getBouncyness());

		// Intersection type: List
		ParticleUniverse::BaseCollider::IntersectionType intersectionType = sphereCollider->getIntersectionType();
		QString intersectionTypeString = IST_POINT;
		if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX) {
			intersectionTypeString = IST_BOX;
		}
		setEnumString(PRNL_INTERSECTION_TYPE, intersectionTypeString);

		// Collision type: List
		ParticleUniverse::BaseCollider::CollisionType collisionType = sphereCollider->getCollisionType();
		QString collisionTypeString = COLLT_NONE;
		if (collisionType == ParticleUniverse::BaseCollider::CT_BOUNCE) {
			collisionTypeString = COLLT_BOUNCE;
		} else if (collisionType == ParticleUniverse::BaseCollider::CT_FLOW) {
			collisionTypeString = COLLT_FLOW;
		}
		setEnumString(PRNL_COLLISION_TYPE, collisionTypeString);
	}

	void SphereColliderPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::SphereCollider * affector = static_cast<ParticleUniverse::SphereCollider *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_SPHERE_COLLIDER_RADIUS) {
			// Radius: ParticleUniverse::Real
			affector->setRadius(prop->getDouble());
		} else if (propertyName == PRNL_SPHERE_COLLIDER_INNER) {
			// Inner Collision: bool
			affector->setInnerCollision(prop->getBool());
		} else if (propertyName == PRNL_COLLIDER_FRICTION) {
			// Friction: ParticleUniverse::Real
			affector->setFriction(prop->getDouble());
		} else if (propertyName == PRNL_COLLIDER_BOUNCYNESS) {
			// Bouncyness: ParticleUniverse::Real
			affector->setBouncyness(prop->getDouble());
		} else if (propertyName == PRNL_INTERSECTION_TYPE) {
			// Intersection type: List
			QString intersection = prop->getEnumString();
			if (intersection == IST_POINT) {
				affector->setIntersectionType(ParticleUniverse::BaseCollider::IT_POINT);
			} else if (intersection == IST_BOX) {
				affector->setIntersectionType(ParticleUniverse::BaseCollider::IT_BOX);
			}
		} else if (propertyName == PRNL_COLLISION_TYPE) {
			// Collision type: List
			QString collision = prop->getEnumString();
			if (collision == COLLT_BOUNCE) {
				affector->setCollisionType(ParticleUniverse::BaseCollider::CT_BOUNCE);
			} else if (collision == COLLT_FLOW) {
				affector->setCollisionType(ParticleUniverse::BaseCollider::CT_FLOW);
			} else if (collision == COLLT_NONE) {
				affector->setCollisionType(ParticleUniverse::BaseCollider::CT_NONE);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
