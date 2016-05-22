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

#include "widgets/ExternPropertyWindows/BoxColliderExternPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"
#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"

#include "Externs/ParticleUniverseBoxColliderExtern.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	BoxColliderExternPropertyWindow::BoxColliderExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ExternPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		// TODO
		PRNL_COLLIDER_FRICTION = QApplication::tr("Friction");
		PRNL_COLLIDER_BOUNCYNESS = QApplication::tr("Bouncyness");
		PRNL_INTERSECTION_TYPE = QApplication::tr("Intersection type");
		PRNL_COLLISION_TYPE = QApplication::tr("Collision type");
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";
		PRNL_EXTERN_THRESHOLD = QApplication::tr("Distance threshold");

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

	void BoxColliderExternPropertyWindow::copyAttributeToExtern(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::Extern * ext = static_cast<ParticleUniverse::Extern *>(_owner->getPUElement());
		ParticleUniverse::BoxColliderExtern * externObject = static_cast<ParticleUniverse::BoxColliderExtern *>(ext);

		if (propertyName == PRNL_EXTERN_THRESHOLD) {
			// Distance Threshold: ParticleUniverse::Real
			ParticleUniverse::Attachable * attachable = static_cast<ParticleUniverse::Attachable *>(_owner->getPUElement());
			attachable->setDistanceThreshold(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_WIDTH) {
			// Width: ParticleUniverse::Real
			externObject->setWidth(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_HEIGHT) {
			// Height: ParticleUniverse::Real
			externObject->setHeight(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_DEPTH) {
			// Depth: ParticleUniverse::Real
			externObject->setDepth(prop->getDouble());
		} else if (propertyName == PRNL_COLLIDER_FRICTION) {
			// Friction: ParticleUniverse::Real
			externObject->setFriction(prop->getDouble());
		} else if (propertyName == PRNL_COLLIDER_BOUNCYNESS) {
			// Bouncyness: ParticleUniverse::Real
			externObject->setBouncyness(prop->getDouble());
		} else if (propertyName == PRNL_INTERSECTION_TYPE) {
			// Intersection type: List
			QString intersection = prop->getEnumString();
			if (intersection == IST_POINT) {
				externObject->setIntersectionType(ParticleUniverse::BaseCollider::IT_POINT);
			} else if (intersection == IST_BOX) {
				externObject->setIntersectionType(ParticleUniverse::BaseCollider::IT_BOX);
			}
		} else if (propertyName == PRNL_COLLISION_TYPE) {
			// Collision type: List
			QString collision = prop->getEnumString();
			if (collision == COLLT_BOUNCE) {
				externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_BOUNCE);
			} else if (collision == COLLT_FLOW) {
				externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_FLOW);
			} else if (collision == COLLT_NONE) {
				externObject->setCollisionType(ParticleUniverse::BaseCollider::CT_NONE);
			}
		} else {
			// Update extern with another attribute
			ExternPropertyWindow::copyAttributeToExtern(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
