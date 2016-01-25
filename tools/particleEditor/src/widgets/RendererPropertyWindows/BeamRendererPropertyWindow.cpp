#include "widgets/RendererPropertyWindows/BeamRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleRenderers/ParticleUniverseBeamRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BeamRendererPropertyWindow::BeamRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : RendererPropertyWindow(par, owner, name) {
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

	void BeamRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		RendererPropertyWindow::copyAttributesFromRenderer(renderer);

		// Copy properties from renderer to property window
		ParticleUniverse::BeamRenderer * beamRenderer = static_cast<ParticleUniverse::BeamRenderer *>(renderer);

		// Use Vertex Colours: bool
		setBool(PRNL_USE_VERTEX_COLOURS, beamRenderer->isUseVertexColours());

		// Max Elements: size_t
		setUint16(PRNL_MAX_ELEMENTS, int64_t(beamRenderer->getMaxChainElements()));

		// Update Interval: ParticleUniverse::Real
		setDouble(PRNL_UPDATE_INTERVAL, beamRenderer->getUpdateInterval());

		// Deviation: ParticleUniverse::Real
		setDouble(PRNL_DEVIATION, beamRenderer->getDeviation());

		// Number Of Segments: size_t
		setUint16(PRNL_NUMBER_OF_SEGMENTS, int64_t(beamRenderer->getNumberOfSegments()));

		// Number Of Segments: size_t
		setBool(PRNL_JUMP, beamRenderer->isJump());

		// Accurate Facing: List
		QString textureDirection = TEXTURE_DIRECTION_U;
		if (beamRenderer->getTexCoordDirection() == Ogre::BillboardChain::TCD_V) {
			textureDirection = TEXTURE_DIRECTION_V;
		}
		setEnumString(PRNL_TEXTURE_DIRECTION, textureDirection);
	}

	void BeamRendererPropertyWindow::copyAttributeToRenderer(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::BeamRenderer * renderer = static_cast<ParticleUniverse::BeamRenderer *>(_owner->getPUElement());
		if (!renderer) {
			return;
		}

		if (propertyName == PRNL_USE_VERTEX_COLOURS) {
			// Use Vertex Colours: bool
			renderer->setUseVertexColours(prop->getBool());
		} else if (propertyName == PRNL_MAX_ELEMENTS) {
			// Max Elements: size_t
			renderer->setMaxChainElements(size_t(prop->getUInt()));
		} else if (propertyName == PRNL_UPDATE_INTERVAL) {
			// Update Interval: ParticleUniverse::Real
			renderer->setUpdateInterval(prop->getDouble());
		} else if (propertyName == PRNL_DEVIATION) {
			// Deviation: ParticleUniverse::Real
			renderer->setDeviation(prop->getDouble());
		} else if (propertyName == PRNL_NUMBER_OF_SEGMENTS) {
			// Number Of Segments: size_t
			renderer->setNumberOfSegments(size_t(prop->getUInt()));
		} else if (propertyName == PRNL_JUMP) {
			// Jump: size_t
			renderer->setJump(prop->getBool());
		} else if (propertyName == PRNL_TEXTURE_DIRECTION) {
			// Texture Direction: List
			QString textureDirection = prop->getEnumString();
			if (textureDirection == TEXTURE_DIRECTION_U) {
				renderer->setTexCoordDirection(Ogre::BillboardChain::TCD_U);
			} else if (textureDirection == TEXTURE_DIRECTION_V) {
				renderer->setTexCoordDirection(Ogre::BillboardChain::TCD_V);
			}
		} else {
			// Update renderer with another attribute
			RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
