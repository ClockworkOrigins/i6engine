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

#include "widgets/AffectorPropertyWindows/BoxColliderPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniverseBoxCollider.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	BoxColliderPropertyWindow::BoxColliderPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_COLLIDER_FRICTION = QApplication::tr("Friction");
		PRNL_COLLIDER_BOUNCYNESS = QApplication::tr("Bouncyness");
		PRNL_INTERSECTION_TYPE = QApplication::tr("Intersection type");
		PRNL_COLLISION_TYPE = QApplication::tr("Collision type");
		IST_POINT = "Point";
		IST_BOX = "Box";
		COLLT_NONE = "None";
		COLLT_BOUNCE = "Bounce";
		COLLT_FLOW = "Flow";
		PRNL_BOX_COLLIDER_WIDTH = QApplication::tr("Box width");
		PRNL_BOX_COLLIDER_HEIGHT = QApplication::tr("Box height");
		PRNL_BOX_COLLIDER_DEPTH = QApplication::tr("Box depth");
		PRNL_BOX_COLLIDER_INNER = QApplication::tr("Inner Collision");

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

	void BoxColliderPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::BoxCollider * boxCollider = static_cast<ParticleUniverse::BoxCollider *>(affector);

		// Width: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_WIDTH, boxCollider->getWidth());

		// Height: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_HEIGHT, boxCollider->getHeight());

		// Depth: ParticleUniverse::Real
		setDouble(PRNL_BOX_COLLIDER_DEPTH, boxCollider->getDepth());

		// Inner Collision: bool
		setBool(PRNL_BOX_COLLIDER_INNER, boxCollider->isInnerCollision());

		// Friction: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_FRICTION, boxCollider->getFriction());

		// Bouncyness: ParticleUniverse::Real
		setDouble(PRNL_COLLIDER_BOUNCYNESS, boxCollider->getBouncyness());

		// Intersection type: List
		ParticleUniverse::BaseCollider::IntersectionType intersectionType = boxCollider->getIntersectionType();
		QString intersectionTypeString = IST_POINT;
		if (intersectionType == ParticleUniverse::BaseCollider::IT_BOX) {
			intersectionTypeString = IST_BOX;
		}
		setEnumString(PRNL_INTERSECTION_TYPE, intersectionTypeString);

		// Collision type: List
		ParticleUniverse::BaseCollider::CollisionType collisionType = boxCollider->getCollisionType();
		QString collisionTypeString = COLLT_NONE;
		if (collisionType == ParticleUniverse::BaseCollider::CT_BOUNCE) {
			collisionTypeString = COLLT_BOUNCE;
		} else if (collisionType == ParticleUniverse::BaseCollider::CT_FLOW) {
			collisionTypeString = COLLT_FLOW;
		}
		setEnumString(PRNL_COLLISION_TYPE, collisionTypeString);
	}

	void BoxColliderPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop)
			return;

		ParticleUniverse::BoxCollider * affector = static_cast<ParticleUniverse::BoxCollider *>(_owner->getPUElement());
		if (!affector)
			return;

		if (propertyName == PRNL_BOX_COLLIDER_WIDTH) {
			// Width: ParticleUniverse::Real
			affector->setWidth(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_HEIGHT) {
			// Height: ParticleUniverse::Real
			affector->setHeight(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_DEPTH) {
			// Depth: ParticleUniverse::Real
			affector->setDepth(prop->getDouble());
		} else if (propertyName == PRNL_BOX_COLLIDER_INNER) {
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
