#include "widgets/RendererPropertyWindows/LightRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/ColourWithAlphaProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleRenderers/ParticleUniverseLightRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	LightRendererPropertyWindow::LightRendererPropertyWindow(QWidget * par, QString name) : RendererPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_LIGHT_TYPE = "Light type";
		LT_POINT = "Point";
		LT_SPOT = "Spot";
		PRNL_ATT_RANGE = "Attenuation range";
		PRNL_ATT_CONSTANT = "Attenuation constant";
		PRNL_ATT_LINEAR = "Attenuation linear";
		PRNL_ATT_QUADRATIC = "Attenuation quadratic";
		PRNL_SPOT_INNER_ANGLE = "spot inner angle";
		PRNL_SPOT_OUTER_ANGLE = "spot outer angle";
		PRNL_FALLOFF = "Falloff";
		PRNL_POWER_SCALE = "Powerscale";
		PRNL_FLASH_FREQUENCY = "Flash frequency";
		PRNL_FLASH_LENGTH = "Flash length";
		PRNL_FLASH_RANDOM = "Flash random";
		PRNL_SPECULAR_COLOUR = "Specular";
		PRNL_POST_RGB = " RGB";
		PRNL_POST_ALPHA = " Alpha";

		// Light Type: List
		QStringList lightTypes;
		lightTypes.append(LT_POINT);
		lightTypes.append(LT_SPOT);
		append(new properties::EnumProperty(this, PRNL_LIGHT_TYPE, PRNL_LIGHT_TYPE, lightTypes));

		// Specular Colour: Ogre::Colour
		append(new properties::ColourWithAlphaProperty(this, PRNL_SPECULAR_COLOUR, PRNL_SPECULAR_COLOUR, Vec4(255, 255, 255, 255)));

		// Attenuation Range: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ATT_RANGE, PRNL_ATT_RANGE, ParticleUniverse::LightRenderer::DEFAULT_ATT_RANGE));

		// Attenuation Constant: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ATT_CONSTANT, PRNL_ATT_CONSTANT, ParticleUniverse::LightRenderer::DEFAULT_ATT_CONSTANT));

		// Attenuation Linear: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ATT_LINEAR, PRNL_ATT_LINEAR, ParticleUniverse::LightRenderer::DEFAULT_ATT_LINEAR));

		// Attenuation Quadratic: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ATT_QUADRATIC, PRNL_ATT_QUADRATIC, ParticleUniverse::LightRenderer::DEFAULT_ATT_QUADRATIC));

		// Spotlight Inner Angle: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPOT_INNER_ANGLE, PRNL_SPOT_INNER_ANGLE, 0.0f));

		// Spotlight Outer Angle: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_SPOT_OUTER_ANGLE, PRNL_SPOT_OUTER_ANGLE, 0.0f));

		// Falloff: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_FALLOFF, PRNL_FALLOFF, ParticleUniverse::LightRenderer::DEFAULT_FALLOFF));

		// Power Scale: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_POWER_SCALE, PRNL_POWER_SCALE, ParticleUniverse::LightRenderer::DEFAULT_POWER_SCALE));

		// Flash Frequency: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_FLASH_FREQUENCY, PRNL_FLASH_FREQUENCY, 0.0f));

		// Flash Length: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_FLASH_LENGTH, PRNL_FLASH_LENGTH, 0.0f));

		// Flash Random: bool
		append(new properties::BoolProperty(this, PRNL_FLASH_RANDOM, PRNL_FLASH_RANDOM, false));
	}

	LightRendererPropertyWindow::~LightRendererPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
