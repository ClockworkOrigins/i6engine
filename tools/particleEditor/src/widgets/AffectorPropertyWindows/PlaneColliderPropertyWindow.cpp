/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "widgets/AffectorPropertyWindows/PlaneColliderPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniversePlaneCollider.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	PlaneColliderPropertyWindow::PlaneColliderPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_PLANE_COLLIDER_NORMAL = QApplication::tr("Plane normal");
		PRNL_COLLIDER_FRICTION = QApplication::tr("Friction");
		PRNL_COLLIDER_BOUNCYNESS = QApplication::tr("Bouncyness");
		PRNL_INTERSECTION_TYPE = QApplication::tr("Intersection type");
		PRNL_COLLISION_TYPE = QApplication::tr("Collision type");
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";

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

	void PlaneColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::PlaneCollider * planeCollider = static_cast<ParticleUniverse::PlaneCollider *>(affector);

		// Normal: Ogre::Vector3
		setVector3(PRNL_PLANE_COLLIDER_NORMAL, planeCollider->getNormal());

		// Friction: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_FRICTION, planeCollider->getFriction());

		// Bouncyness: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_BOUNCYNESS, planeCollider->getBouncyness());

		// Intersection type: List
		ParticleUniverse::BaseCollider::IntersectionType intersectionType = planeCollider->getIntersectionType();
		QString intersectionTypeString = IST_POINT;
		if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX) {
			intersectionTypeString = IST_BOX;
		}
		setEnumString(PRNL_INTERSECTION_TYPE, intersectionTypeString);

		// Collision type: List
		ParticleUniverse::BaseCollider::CollisionType collisionType = planeCollider->getCollisionType();
		QString collisionTypeString = COLLT_NONE;
		if (collisionType == ParticleUniverse::BaseCollider::CT_BOUNCE) {
			collisionTypeString = COLLT_BOUNCE;
		} else if (collisionType == ParticleUniverse::BaseCollider::CT_FLOW) {
			collisionTypeString = COLLT_FLOW;
		}
		setEnumString(PRNL_COLLISION_TYPE, collisionTypeString);
	}

	void PlaneColliderPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::PlaneCollider * affector = static_cast<ParticleUniverse::PlaneCollider *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_PLANE_COLLIDER_NORMAL) {
			// Normal: Ogre::Vector3
			affector->setNormal(prop->getVector3());
		} else if (propertyName == PRNL_AFFECTOR_POSITION) {
			// Position: Ogre::Vector3
			affector->position = prop->getVector3();
			affector->originalPosition = prop->getVector3();

			// To activate the new position, reset the plane normal
			Ogre::Vector3 normal = affector->getNormal();
			affector->setNormal(normal);
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
