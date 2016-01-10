#include "widgets/RendererPropertyWindows/RibbonTrailRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/ColourWithAlphaProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/UIntProperty.h"

#include "ParticleRenderers/ParticleUniverseRibbonTrailRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	RibbonTrailRendererPropertyWindow::RibbonTrailRendererPropertyWindow(QWidget * par, QString name) : RendererPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_USE_VERTEX_COLOURS = "Use vertex colours";
		PRNL_MAX_ELEMENTS = "Maximum elements";
		PRNL_TRAIL_LENGTH = "Trail length";
		PRNL_TRAIL_WIDTH = "Trail width";
		PRNL_RANDOM_INITIAL_COLOUR = "Random initial colour";
		PRNL_INITIAL_COLOUR = "Initial colour";
		PRNL_COLOUR_CHANGE = "Colour change";
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";

		// Use Vertex Colours: bool
		append(new properties::BoolProperty(this, PRNL_USE_VERTEX_COLOURS, PRNL_USE_VERTEX_COLOURS, ParticleUniverse::RibbonTrailRenderer::DEFAULT_USE_VERTEX_COLOURS));

		// Max Chain Elements: unsigned int
		append(new properties::UIntProperty(this, PRNL_MAX_ELEMENTS, PRNL_MAX_ELEMENTS, ParticleUniverse::RibbonTrailRenderer::DEFAULT_MAX_ELEMENTS));

		// Trail Length: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TRAIL_LENGTH, PRNL_TRAIL_LENGTH, ParticleUniverse::RibbonTrailRenderer::DEFAULT_LENGTH));

		// Trail Width: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_TRAIL_WIDTH, PRNL_TRAIL_WIDTH, ParticleUniverse::RibbonTrailRenderer::DEFAULT_WIDTH));

		// Random Initial Colour: bool
		append(new properties::BoolProperty(this, PRNL_RANDOM_INITIAL_COLOUR, PRNL_RANDOM_INITIAL_COLOUR, ParticleUniverse::RibbonTrailRenderer::DEFAULT_RANDOM_INITIAL_COLOUR));

		// Initial Colour: Ogre::Colour
		Vec4 initialColour(ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.r, ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.g, ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.b, ParticleUniverse::RibbonTrailRenderer::DEFAULT_INITIAL_COLOUR.a);
		append(new properties::ColourWithAlphaProperty(this, PRNL_INITIAL_COLOUR, PRNL_INITIAL_COLOUR, initialColour));

		// Colour Change: Ogre::Colour
		Vec4 colourChange(ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.r, ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.g, ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.b, ParticleUniverse::RibbonTrailRenderer::DEFAULT_COLOUR_CHANGE.a);
		append(new properties::ColourWithAlphaProperty(this, PRNL_COLOUR_CHANGE, PRNL_COLOUR_CHANGE, colourChange));
	}

	RibbonTrailRendererPropertyWindow::~RibbonTrailRendererPropertyWindow() {
	}

	void RibbonTrailRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		RendererPropertyWindow::copyAttributesFromRenderer(renderer);

		// Copy properties from renderer to property window
		ParticleUniverse::RibbonTrailRenderer * ribbonTrailRenderer = static_cast<ParticleUniverse::RibbonTrailRenderer *>(renderer);

		// Use Vertex Colours: bool
		setBool(PRNL_USE_VERTEX_COLOURS, ribbonTrailRenderer->isUseVertexColours());

		// Max Chain Elements: unsigned int
		setUint16(PRNL_MAX_ELEMENTS, ribbonTrailRenderer->getMaxChainElements());

		// Trail Length: ParticleUniverse::Real
		setDouble(PRNL_TRAIL_LENGTH, ribbonTrailRenderer->getTrailLength());

		// Trail Width: ParticleUniverse::Real
		setDouble(PRNL_TRAIL_WIDTH, ribbonTrailRenderer->getTrailWidth());

		// Random Initial Colour: bool
		setBool(PRNL_RANDOM_INITIAL_COLOUR, ribbonTrailRenderer->isRandomInitialColour());

		// Initial Colour: Ogre::Colour
		Vec4 initialColour(255 * ribbonTrailRenderer->getInitialColour().r, 255 * ribbonTrailRenderer->getInitialColour().g, 255 * ribbonTrailRenderer->getInitialColour().b, 255 * ribbonTrailRenderer->getInitialColour().a);
		setColourWithAlpha(PRNL_INITIAL_COLOUR, initialColour);

		// Colour Change: Ogre::Colour
		Vec4 colourChange(255 * ribbonTrailRenderer->getColourChange().r, 255 * ribbonTrailRenderer->getColourChange().g, 255 * ribbonTrailRenderer->getColourChange().b, 255 * ribbonTrailRenderer->getColourChange().a);
		setColourWithAlpha(PRNL_COLOUR_CHANGE, colourChange);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
