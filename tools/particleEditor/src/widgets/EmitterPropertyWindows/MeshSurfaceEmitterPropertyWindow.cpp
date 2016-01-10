#include "widgets/EmitterPropertyWindows/MeshSurfaceEmitterPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/StringProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleEmitters/ParticleUniverseMeshSurfaceEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	MeshSurfaceEmitterPropertyWindow::MeshSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";
		PRNL_MESH_SURFACE_EMITTER_NAME = "Mesh name";
		PRNL_MESH_SURFACE_EMITTER_DISTRIBUTION = "Mesh surface distribution";
		PRNL_MESH_SURFACE_EMITTER_SCALE = "Mesh surface scale";
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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
