#include "widgets/EmitterPropertyWindows/VertexEmitterPropertyWindow.h"

#include "properties/StringProperty.h"
#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleEmitters/ParticleUniverseVertexEmitter.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	VertexEmitterPropertyWindow::VertexEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : EmitterPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_VERTEX_EMITTER_STEP = QApplication::tr("Vertex step");
		PRNL_VERTEX_EMITTER_SEGMENTS = QApplication::tr("Vertex segments");
		PRNL_VERTEX_EMITTER_ITERATION = QApplication::tr("Vertex iterations");
		PRNL_VERTEX_EMITTER_MESH_NAME = QApplication::tr("Vertex meshname");

		// Step: ParticleUniverse::uint
		append(new properties::UIntProperty(this, PRNL_VERTEX_EMITTER_STEP, PRNL_VERTEX_EMITTER_STEP, ParticleUniverse::VertexEmitter::DEFAULT_STEP));

		// Segments: ParticleUniverse::uint
		append(new properties::UIntProperty(this, PRNL_VERTEX_EMITTER_SEGMENTS, PRNL_VERTEX_EMITTER_SEGMENTS, ParticleUniverse::VertexEmitter::DEFAULT_SEGMENTS));

		// Iterations: ParticleUniverse::uint
		append(new properties::UIntProperty(this, PRNL_VERTEX_EMITTER_ITERATION, PRNL_VERTEX_EMITTER_ITERATION, ParticleUniverse::VertexEmitter::DEFAULT_ITERATIONS));

		// Step: Ogre::String
		append(new properties::StringProperty(this, PRNL_VERTEX_EMITTER_MESH_NAME, PRNL_VERTEX_EMITTER_MESH_NAME, ""));
	}

	VertexEmitterPropertyWindow::~VertexEmitterPropertyWindow() {
	}

	void VertexEmitterPropertyWindow::copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) {
		EmitterPropertyWindow::copyAttributesFromEmitter(emitter);

		// Copy properties from emitter to property window
		ParticleUniverse::VertexEmitter * vertexEmitter = static_cast<ParticleUniverse::VertexEmitter *>(emitter);

		// Step: ParticleUniverse::uint
		setUint16(PRNL_VERTEX_EMITTER_STEP, vertexEmitter->getStep());

		// Segments: ParticleUniverse::uint
		setUint16(PRNL_VERTEX_EMITTER_SEGMENTS, vertexEmitter->getSegments());

		// Iterations: ParticleUniverse::uint
		setUint16(PRNL_VERTEX_EMITTER_ITERATION, vertexEmitter->getIterations());

		// Step: Ogre::String
		setString(PRNL_VERTEX_EMITTER_MESH_NAME, QString::fromStdString(vertexEmitter->getMeshName()));
	}

	void VertexEmitterPropertyWindow::copyAttributeToEmitter(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::VertexEmitter * vertexEmitter = static_cast<ParticleUniverse::VertexEmitter *>(_owner->getPUElement());
		if (!vertexEmitter) {
			return;
		}

		if (propertyName == PRNL_VERTEX_EMITTER_STEP) {
			// Update emitter with Step
			vertexEmitter->setStep(prop->getUInt());
		} else if (propertyName == PRNL_VERTEX_EMITTER_SEGMENTS) {
			// Update emitter with Segments
			vertexEmitter->setSegments(prop->getUInt());
		} else if (propertyName == PRNL_VERTEX_EMITTER_ITERATION) {
			// Update emitter with Iteration
			vertexEmitter->setIterations(prop->getUInt());
		} else if (propertyName == PRNL_VERTEX_EMITTER_MESH_NAME) {
			// Update emitter with Mesh name
			Ogre::String meshName = prop->getString().toStdString();
			if (meshName != Ogre::StringUtil::BLANK) {
				vertexEmitter->setMeshName(meshName);
			}
		} else {
			// Update emitter with another attribute
			EmitterPropertyWindow::copyAttributeToEmitter(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
