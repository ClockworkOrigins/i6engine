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

#include "widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/StringProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseMeshSurfaceEmitter.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	MeshSurfaceEmitterPropertyWindow::MeshSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_MESH_SURFACE_EMITTER_NAME = QApplication::tr("Mesh name");
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION = QApplication::tr("Mesh surface distribution");
		PRNL_MESH_SURFACE_EMITTER_SCALE = QApplication::tr("Mesh surface scale");
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE = "Edge";
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1 = "Heterogeneous_1";
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2 = "Heterogeneous_2";
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS = "Homogeneus";
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX = "Vertex";

		// Mesh name: String
		append(new properties::StringProperty(this, PRNL_MESH_SURFACE_EMITTER_NAME, PRNL_MESH_SURFACE_EMITTER_NAME, ""));

		// Distribution: List
		QStringList distribution;
		distribution.append(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE);
		distribution.append(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1);
		distribution.append(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2);
		distribution.append(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS);
		distribution.append(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX);
		append(new properties::EnumProperty(this, PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION, PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION, distribution));

		// Scale: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_MESH_SURFACE_EMITTER_SCALE, PRNL_MESH_SURFACE_EMITTER_SCALE, ParticleUniverse::MeshSurfaceEmitter::DEFAULT_SCALE));
	}

	MeshSurfaceEmitterPropertyWindow::~MeshSurfaceEmitterPropertyWindow() {
	}

	void MeshSurfaceEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::MeshSurfaceEmitter * meshSurfaceEmitter = static_cast<ParticleUniverse::MeshSurfaceEmitter *>(emitter);

		// Mesh name: Ogre::String
		setString(PRNL_MESH_SURFACE_EMITTER_NAME, QString::fromStdString(meshSurfaceEmitter->getMeshName()));

		// Distribution: List
		ParticleUniverse::MeshInfo::MeshSurfaceDistribution distribution = meshSurfaceEmitter->getDistribution();
		QString distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE;
		if (distribution == ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_1) {
			distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1;
		} else if (distribution == ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_2) {
			distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2;
		} else if (distribution == ParticleUniverse::MeshInfo::MSD_HOMOGENEOUS) {
			distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS;
		} else if (distribution == ParticleUniverse::MeshInfo::MSD_VERTEX) {
			distributionString = PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX;
		}
		setEnumString(PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION, distributionString);

		// Scale: Ogre::Vector3
		setVector3(PRNL_MESH_SURFACE_EMITTER_SCALE, meshSurfaceEmitter->getScale());
	}

	void MeshSurfaceEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::MeshSurfaceEmitter * meshSurfaceEmitter = static_cast<ParticleUniverse::MeshSurfaceEmitter *>(_owner->getPUElement());
		if (!meshSurfaceEmitter) {
			return;
		}

		if (propertyName == PRNL_MESH_SURFACE_EMITTER_NAME) {
			// Update emitter with Mesh name
			Ogre::String meshName = prop->getString().toStdString();
			if (meshName != Ogre::StringUtil::BLANK) {
				meshSurfaceEmitter->setMeshName(meshName); // No explicit build needed, because the setMeshName() already rebuilds it.
			}
		} else if (propertyName == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION) {
			// Update emitter with Distribution
			QString distribution = prop->getEnumString();
			if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_EDGE) {
				meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_EDGE);
				meshSurfaceEmitter->build();
			} else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_1) {
				meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_1);
				meshSurfaceEmitter->build();
			} else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HETRG_2) {
				meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HETEROGENEOUS_2);
				meshSurfaceEmitter->build();
			} else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_HOMOGENEUS) {
				meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_HOMOGENEOUS);
				meshSurfaceEmitter->build();
			} else if (distribution == PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION_VERTEX) {
				meshSurfaceEmitter->setDistribution(ParticleUniverse::MeshInfo::MSD_VERTEX);
				meshSurfaceEmitter->build();
			}
		} else if (propertyName == PRNL_MESH_SURFACE_EMITTER_SCALE) {
			// Update emitter with Scale
			meshSurfaceEmitter->setScale(prop->getVector3());
			meshSurfaceEmitter->build();
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
