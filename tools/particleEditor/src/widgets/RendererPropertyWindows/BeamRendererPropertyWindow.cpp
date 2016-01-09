#include "widgets/RendererPropertyWindows/BeamRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "ParticleRenderers/ParticleUniverseBeamRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BeamRendererPropertyWindow::BeamRendererPropertyWindow(QWidget * par, QString name) : RendererPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_USE_VERTEX_COLOURS = "Use vertex colours";
		PRNL_UPDATE_INTERVAL = "Update interval";
		PRNL_MAX_ELEMENTS = "Maximum elements";
		PRNL_DEVIATION = "Deviation";
		PRNL_NUMBER_OF_SEGMENTS = "Number of segments";
		PRNL_JUMP = "Jump";
		PRNL_TEXTURE_DIRECTION = "Texture direction";
		TEXTURE_DIRECTION_U = "TCD_U";
		TEXTURE_DIRECTION_V = "TCD_V";

		// Use Vertex Colours: bool
		append(new properties::BoolProperty(this, PRNL_USE_VERTEX_COLOURS, PRNL_USE_VERTEX_COLOURS, ParticleUniverse::BeamRenderer::DEFAULT_USE_VERTEX_COLOURS));

		// Max Elements: size_t
		append(new properties::UIntProperty(this, PRNL_MAX_ELEMENTS, PRNL_MAX_ELEMENTS, ParticleUniverse::BeamRenderer::DEFAULT_MAX_ELEMENTS));

		// Update Interval: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_UPDATE_INTERVAL, PRNL_UPDATE_INTERVAL, ParticleUniverse::BeamRenderer::DEFAULT_UPDATE_INTERVAL));

		// Deviation: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_DEVIATION, PRNL_DEVIATION, ParticleUniverse::BeamRenderer::DEFAULT_DEVIATION));

		// Number Of Segments: size_t
		append(new properties::UIntProperty(this, PRNL_NUMBER_OF_SEGMENTS, PRNL_NUMBER_OF_SEGMENTS, ParticleUniverse::BeamRenderer::DEFAULT_NUMBER_OF_SEGMENTS));

		// Jump: bool
		append(new properties::BoolProperty(this, PRNL_JUMP, PRNL_JUMP, false));

		// Texture Direction: List
		QStringList textureDirection;
		textureDirection.append(TEXTURE_DIRECTION_U);
		textureDirection.append(TEXTURE_DIRECTION_V);
		properties::EnumProperty * prop = new properties::EnumProperty(this, PRNL_TEXTURE_DIRECTION, PRNL_TEXTURE_DIRECTION, textureDirection);
		append(prop);
		prop->setCurrentIndex(1);
	}

	BeamRendererPropertyWindow::~BeamRendererPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
