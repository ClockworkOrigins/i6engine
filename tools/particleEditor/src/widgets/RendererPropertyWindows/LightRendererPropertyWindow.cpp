#include "widgets/RendererPropertyWindows/LightRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/ColourWithAlphaProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleRenderers/ParticleUniverseLightRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	LightRendererPropertyWindow::LightRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : RendererPropertyWindow(par, owner, name) {
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

	void LightRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		RendererPropertyWindow::copyAttributesFromRenderer(renderer);

		// Copy properties from renderer to property window
		ParticleUniverse::LightRenderer * lightRenderer = static_cast<ParticleUniverse::LightRenderer *>(renderer);

		// Light Type: List
		Ogre::Light::LightTypes lightType = lightRenderer->getLightType();
		QString lightTypeString = LT_POINT;
		if (lightType == Ogre::Light::LT_SPOTLIGHT) {
			lightTypeString = LT_SPOT;
		}
		setEnumString(PRNL_LIGHT_TYPE, lightTypeString);

		// Specular Colour: Ogre::Colour
		Vec4 specularColour(255 * lightRenderer->getSpecularColour().r, 255 * lightRenderer->getSpecularColour().g, 255 * lightRenderer->getSpecularColour().b, 255 * lightRenderer->getSpecularColour().a);
		setColourWithAlpha(PRNL_SPECULAR_COLOUR, specularColour);

		// Attenuation Range: ParticleUniverse::Real
		setDouble(PRNL_ATT_RANGE, lightRenderer->getAttenuationRange());

		// Attenuation Constant: ParticleUniverse::Real
		setDouble(PRNL_ATT_CONSTANT, lightRenderer->getAttenuationConstant());

		// Attenuation Linear: ParticleUniverse::Real
		setDouble(PRNL_ATT_LINEAR, lightRenderer->getAttenuationLinear());

		// Attenuation Quadratic: ParticleUniverse::Real
		setDouble(PRNL_ATT_QUADRATIC, lightRenderer->getAttenuationQuadratic());

		// Spotlight Inner Angle: ParticleUniverse::Real
		setDouble(PRNL_SPOT_INNER_ANGLE, lightRenderer->getSpotlightInnerAngle().valueDegrees());

		// Spotlight Outer Angle: ParticleUniverse::Real
		setDouble(PRNL_SPOT_OUTER_ANGLE, lightRenderer->getSpotlightOuterAngle().valueDegrees());

		// Falloff: ParticleUniverse::Real
		setDouble(PRNL_FALLOFF, lightRenderer->getSpotlightFalloff());

		// Power Scale: ParticleUniverse::Real
		setDouble(PRNL_POWER_SCALE, lightRenderer->getPowerScale());

		// Flash Frequency: ParticleUniverse::Real
		setDouble(PRNL_FLASH_FREQUENCY, lightRenderer->getFlashFrequency());

		// Flash Length: ParticleUniverse::Real
		setDouble(PRNL_FLASH_LENGTH, lightRenderer->getFlashLength());

		// Flash Length: ParticleUniverse::Real
		setBool(PRNL_FLASH_RANDOM, lightRenderer->isFlashRandom());
	}


	void LightRendererPropertyWindow::copyAttributeToRenderer(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::LightRenderer * renderer = static_cast<ParticleUniverse::LightRenderer *>(_owner->getPUElement());
		if (!renderer) {
			return;
		}

		if (propertyName == PRNL_LIGHT_TYPE) {
			// Light Type: List
			QString lightType = prop->getEnumString();
			if (lightType == LT_POINT) {
				renderer->setLightType(Ogre::Light::LT_POINT);
			} else if (lightType == LT_SPOT) {
				renderer->setLightType(Ogre::Light::LT_SPOTLIGHT);
			}
		} else if (propertyName == PRNL_SPECULAR_COLOUR) {
			// Specular Colour: Ogre::Colour
			Vec4 c = prop->getColourWithAlpha();
			Ogre::ColourValue colour(ParticleUniverse::Real(c.getX()) / 255.0f, ParticleUniverse::Real(c.getY()) / 255.0f, ParticleUniverse::Real(c.getZ()) / 255.0f, ParticleUniverse::Real(c.getW()) / 255.0f);
			renderer->setSpecularColour(colour);
		} else if (propertyName == PRNL_ATT_RANGE) {
			// Attenuation Range: ParticleUniverse::Real
			renderer->setAttenuationRange(prop->getDouble());
		} else if (propertyName == PRNL_ATT_CONSTANT) {
			// Attenuation Constant: ParticleUniverse::Real
			renderer->setAttenuationConstant(prop->getDouble());
		} else if (propertyName == PRNL_ATT_LINEAR) {
			// Attenuation Linear: ParticleUniverse::Real
			renderer->setAttenuationLinear(prop->getDouble());
		} else if (propertyName == PRNL_ATT_QUADRATIC) {
			// Attenuation Quadratic: ParticleUniverse::Real
			renderer->setAttenuationQuadratic(prop->getDouble());
		} else if (propertyName == PRNL_SPOT_INNER_ANGLE) {
			// Spotlight Inner Angle: ParticleUniverse::Real
			renderer->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(prop->getDouble())));
		} else if (propertyName == PRNL_SPOT_OUTER_ANGLE) {
			// Spotlight Outer Angle: ParticleUniverse::Real
			renderer->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(prop->getDouble())));
		} else if (propertyName == PRNL_FALLOFF) {
			// Falloff: ParticleUniverse::Real
			renderer->setSpotlightFalloff(prop->getDouble());
		} else if (propertyName == PRNL_POWER_SCALE) {
			// Power Scale: ParticleUniverse::Real
			renderer->setPowerScale(prop->getDouble());
		} else if (propertyName == PRNL_FLASH_FREQUENCY) {
			// Flash Frequency: ParticleUniverse::Real
			renderer->setFlashFrequency(prop->getDouble());
		} else if (propertyName == PRNL_FLASH_LENGTH) {
			// Flash Length: ParticleUniverse::Real
			renderer->setFlashLength(prop->getDouble());
		} else if (propertyName == PRNL_FLASH_RANDOM) {
			// Flash Random: bool
			renderer->setFlashRandom(prop->getBool());
		} else {
			// Update renderer with another attribute
			RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
