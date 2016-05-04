#include "widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleRenderers/ParticleUniverseBillboardRenderer.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	BillboardRendererPropertyWindow::BillboardRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : RendererPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = QApplication::tr("Renderer type");
		PRNL_BILLBOARD_TYPE = QApplication::tr("Billboard type");
		BBT_POINT = "Point";
		BBT_ORIENTED_COMMON = "Oriented Common";
		BBT_ORIENTED_SELF = "Oriented Self";
		BBT_PERPENDICULAR_COMMON = "Perpendicular Common";
		BBT_PERPENDICULAR_SELF = "Perpendicular Self";
		BBT_ORIENTED_SHAPE = "Oriented Shape";
		PRNL_ORIGIN = QApplication::tr("Origin");
		ORG_TOP_LEFT = "Top-left";
		ORG_TOP_CENTER = "Top-center";
		ORG_TOP_RIGHT = "Top-right";
		ORG_CENTER_LEFT = "Center-left";
		ORG_CENTER_RIGHT = "Center-right";
		ORG_CENTER = "Center";
		ORG_BOTTOM_LEFT = "Bottom-left";
		ORG_BOTTOM_RIGHT = "Bottom-right";
		ORG_BOTTOM_CENTER = "Bottom-center";
		PRNL_ROTATION_TYPE = QApplication::tr("Rotation type");
		RT_VERTEX = "Vertex";
		RT_TEXTURE_COORDINATES = "Texture coordinates";
		PRNL_COMMON_DIRECTION = QApplication::tr("Common direction");
		PRNL_UP_VECTOR = QApplication::tr("Up Vector");
		PRNL_POINT_RENDERING = QApplication::tr("Point Rendering");
		PRNL_ACCURATE_FACING = QApplication::tr("Accurate Facing");
		ACF_ON = "On";
		ACF_OFF = "Off";
		PRNL_RENDERER_USE_SOFT_PARTICLES = QApplication::tr("Use soft particles");
		PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = QApplication::tr("Soft particles contrast power");
		PRNL_RENDERER_SOFT_PARTICLES_SCALE = QApplication::tr("Soft particles scale");
		PRNL_RENDERER_SOFT_PARTICLES_DELTA = QApplication::tr("Soft particles delta");

		// Billboard Type: List
		QStringList billboardTypes;
		billboardTypes.append(BBT_POINT);
		billboardTypes.append(BBT_ORIENTED_COMMON);
		billboardTypes.append(BBT_ORIENTED_SELF);
		billboardTypes.append(BBT_PERPENDICULAR_COMMON);
		billboardTypes.append(BBT_PERPENDICULAR_SELF);
		billboardTypes.append(BBT_ORIENTED_SHAPE);
		append(new properties::EnumProperty(this, PRNL_BILLBOARD_TYPE, PRNL_BILLBOARD_TYPE, billboardTypes));

		// Origin: List
		QStringList origin;
		origin.append(ORG_TOP_LEFT);
		origin.append(ORG_TOP_CENTER);
		origin.append(ORG_TOP_RIGHT);
		origin.append(ORG_CENTER_LEFT);
		origin.append(ORG_CENTER_RIGHT);
		origin.append(ORG_CENTER);
		origin.append(ORG_BOTTOM_LEFT);
		origin.append(ORG_BOTTOM_RIGHT);
		origin.append(ORG_BOTTOM_CENTER);
		append(new properties::EnumProperty(this, PRNL_ORIGIN, PRNL_ORIGIN, origin));

		// Rotation Type: List
		QStringList rotationTypes;
		rotationTypes.append(RT_VERTEX);
		rotationTypes.append(RT_TEXTURE_COORDINATES);
		append(new properties::EnumProperty(this, PRNL_ROTATION_TYPE, PRNL_ROTATION_TYPE, rotationTypes));

		// Common Direction: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_COMMON_DIRECTION, PRNL_COMMON_DIRECTION, ParticleUniverse::BillboardRenderer::DEFAULT_COMMON_DIRECTION));

		// Common Up Vector: Ogre::Vector3
		append(new properties::Vec3Property(this, PRNL_UP_VECTOR, PRNL_UP_VECTOR, ParticleUniverse::BillboardRenderer::DEFAULT_COMMON_UP_VECTOR));

		// Point Rendering: bool
		append(new properties::BoolProperty(this, PRNL_POINT_RENDERING, PRNL_POINT_RENDERING, ParticleUniverse::BillboardRenderer::DEFAULT_POINT_RENDERING));

		// Accurate Facing: List
		QStringList accurateFacing;
		accurateFacing.append(ACF_ON);
		accurateFacing.append(ACF_OFF);
		properties::EnumProperty * prop = new properties::EnumProperty(this, PRNL_ACCURATE_FACING, PRNL_ACCURATE_FACING, accurateFacing);
		append(prop);
		prop->setCurrentIndex(1);

		// Note: Although soft particles are generic renderer properties, they currently only work in combination with a billboard renderer.

		// Use soft particles: Bool
		append(new properties::BoolProperty(this, PRNL_RENDERER_USE_SOFT_PARTICLES, PRNL_RENDERER_USE_SOFT_PARTICLES, ParticleUniverse::ParticleRenderer::DEFAULT_USE_SOFT_PARTICLES));

		// Soft particles contrast power: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_CONTRAST_POWER));

		// Soft particles scale: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_RENDERER_SOFT_PARTICLES_SCALE, PRNL_RENDERER_SOFT_PARTICLES_SCALE, ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_SCALE));

		// Soft particles delta: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_RENDERER_SOFT_PARTICLES_DELTA, PRNL_RENDERER_SOFT_PARTICLES_DELTA, ParticleUniverse::ParticleRenderer::DEFAULT_SOFT_PARTICLES_DELTA));
	}

	BillboardRendererPropertyWindow::~BillboardRendererPropertyWindow() {
	}

	void BillboardRendererPropertyWindow::copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) {
		RendererPropertyWindow::copyAttributesFromRenderer(renderer);

		// Copy properties from renderer to property window
		ParticleUniverse::BillboardRenderer * billboardRenderer = static_cast<ParticleUniverse::BillboardRenderer *>(renderer);

		// Billboard Type: List
		ParticleUniverse::BillboardRenderer::BillboardType billboardType = billboardRenderer->getBillboardType();
		QString billboardTypeString = BBT_POINT;
		if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_COMMON) {
			billboardTypeString = BBT_ORIENTED_COMMON;
		} else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SELF) {
			billboardTypeString = BBT_ORIENTED_SELF;
		} else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_COMMON) {
			billboardTypeString = BBT_PERPENDICULAR_COMMON;
		} else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_SELF) {
			billboardTypeString = BBT_PERPENDICULAR_SELF;
		} else if (billboardType == ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SHAPE) {
			billboardTypeString = BBT_ORIENTED_SHAPE;
		}
		setEnumString(PRNL_BILLBOARD_TYPE, billboardTypeString);

		// Origin: List
		Ogre::BillboardOrigin origin = billboardRenderer->getBillboardOrigin();
		QString originString = ORG_CENTER;
		if (origin == Ogre::BBO_BOTTOM_CENTER) {
			originString = ORG_BOTTOM_CENTER;
		} else if (origin == Ogre::BBO_BOTTOM_LEFT) {
			originString = ORG_BOTTOM_LEFT;
		} else if (origin == Ogre::BBO_BOTTOM_RIGHT) {
			originString = ORG_BOTTOM_RIGHT;
		} else if (origin == Ogre::BBO_CENTER_LEFT) {
			originString = ORG_CENTER_LEFT;
		} else if (origin == Ogre::BBO_CENTER_RIGHT) {
			originString = ORG_CENTER_RIGHT;
		} else if (origin == Ogre::BBO_TOP_CENTER) {
			originString = ORG_TOP_CENTER;
		} else if (origin == Ogre::BBO_TOP_LEFT) {
			originString = ORG_TOP_LEFT;
		} else if (origin == Ogre::BBO_TOP_RIGHT) {
			originString = ORG_TOP_RIGHT;
		}
		setEnumString(PRNL_ORIGIN, originString);

		// Rotation Type: List
		Ogre::BillboardRotationType rotationType = billboardRenderer->getBillboardRotationType();
		QString rotationTypeString = RT_VERTEX;
		if (rotationType == Ogre::BBR_TEXCOORD) {
			rotationTypeString = RT_TEXTURE_COORDINATES;
		}
		setEnumString(PRNL_ROTATION_TYPE, rotationTypeString);

		// Common Direction: Ogre::Vector3
		setVector3(PRNL_COMMON_DIRECTION, billboardRenderer->getCommonDirection());

		// Common Up Vector: Ogre::Vector3
		setVector3(PRNL_UP_VECTOR, billboardRenderer->getCommonUpVector());

		// Point Rendering: bool
		setBool(PRNL_POINT_RENDERING, billboardRenderer->isPointRenderingEnabled());

		// Accurate Facing: List
		QString accurateFacingString = ACF_OFF;
		if (billboardRenderer->isUseAccurateFacing()) {
			accurateFacingString = ACF_ON;
		}
		setEnumString(PRNL_ACCURATE_FACING, accurateFacingString);

		// Use soft particles: Bool
		setBool(PRNL_RENDERER_USE_SOFT_PARTICLES, renderer->getUseSoftParticles());

		// Soft particles contrast power: ParticleUniverse::Real
		setDouble(PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER, renderer->getSoftParticlesContrastPower());

		// Soft particles scale: ParticleUniverse::Real
		setDouble(PRNL_RENDERER_SOFT_PARTICLES_SCALE, renderer->getSoftParticlesScale());

		// Soft particles delta: ParticleUniverse::Real
		setDouble(PRNL_RENDERER_SOFT_PARTICLES_DELTA, renderer->getSoftParticlesDelta());
	}

	void BillboardRendererPropertyWindow::copyAttributeToRenderer(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::BillboardRenderer * renderer = static_cast<ParticleUniverse::BillboardRenderer *>(_owner->getPUElement());
		if (!renderer) {
			return;
		}

		if (propertyName == PRNL_BILLBOARD_TYPE) {
			// Billboard Type: List
			QString bilboardType = prop->getEnumString();
			if (bilboardType == BBT_ORIENTED_COMMON) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_COMMON);
			} else if (bilboardType == BBT_ORIENTED_SELF) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SELF);
			} else if (bilboardType == BBT_ORIENTED_SHAPE) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_ORIENTED_SHAPE);
			} else if (bilboardType == BBT_PERPENDICULAR_COMMON) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_COMMON);
			} else if (bilboardType == BBT_PERPENDICULAR_SELF) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_PERPENDICULAR_SELF);
			} else if (bilboardType == BBT_POINT) {
				renderer->setBillboardType(ParticleUniverse::BillboardRenderer::BBT_POINT);
			}
		} else if (propertyName == PRNL_ORIGIN) {
			// Origin: List
			QString origin = prop->getEnumString();
			if (origin == ORG_BOTTOM_CENTER) {
				renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_CENTER);
			} else if (origin == ORG_BOTTOM_LEFT) {
				renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_LEFT);
			} else if (origin == ORG_BOTTOM_RIGHT) {
				renderer->setBillboardOrigin(Ogre::BBO_BOTTOM_RIGHT);
			} else if (origin == ORG_CENTER) {
				renderer->setBillboardOrigin(Ogre::BBO_CENTER);
			} else if (origin == ORG_CENTER_LEFT) {
				renderer->setBillboardOrigin(Ogre::BBO_CENTER_LEFT);
			} else if (origin == ORG_CENTER_RIGHT) {
				renderer->setBillboardOrigin(Ogre::BBO_CENTER_RIGHT);
			} else if (origin == ORG_TOP_CENTER) {
				renderer->setBillboardOrigin(Ogre::BBO_TOP_CENTER);
			} else if (origin == ORG_TOP_LEFT) {
				renderer->setBillboardOrigin(Ogre::BBO_TOP_LEFT);
			} else if (origin == ORG_TOP_RIGHT) {
				renderer->setBillboardOrigin(Ogre::BBO_TOP_RIGHT);
			}
		} else if (propertyName == PRNL_ROTATION_TYPE) {
			// Rotation Type: List
			QString rotationType = prop->getEnumString();
			if (rotationType == RT_VERTEX) {
				renderer->setBillboardRotationType(Ogre::BBR_VERTEX);
			} else if (rotationType == RT_TEXTURE_COORDINATES) {
				renderer->setBillboardRotationType(Ogre::BBR_TEXCOORD);
			}
		} else if (propertyName == PRNL_COMMON_DIRECTION) {
			// Common Direction: Ogre::Vector3
			renderer->setCommonDirection(prop->getVector3());
		} else if (propertyName == PRNL_UP_VECTOR) {
			// Common Up Vector: Ogre::Vector3
			renderer->setCommonUpVector(prop->getVector3().normalisedCopy());
		} else if (propertyName == PRNL_POINT_RENDERING) {
			// Point Rendering: bool
			renderer->setPointRenderingEnabled(prop->getBool());
		} else if (propertyName == PRNL_ACCURATE_FACING) {
			// Accurate Facing: List
			QString accurateFacing = prop->getEnumString();
			if (accurateFacing == ACF_ON) {
				renderer->setUseAccurateFacing(true);
			} else if (accurateFacing == ACF_OFF) {
				renderer->setUseAccurateFacing(false);
			}
		} else if (propertyName == PRNL_RENDERER_USE_SOFT_PARTICLES) {
			// Use soft particles: Bool
			renderer->setUseSoftParticles(prop->getBool());
		} else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER) {
			// Soft particles contrast power: ParticleUniverse::Real
			renderer->setSoftParticlesContrastPower(prop->getDouble());
		} else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_SCALE) {
			// Soft particles scale: ParticleUniverse::Real
			renderer->setSoftParticlesScale(prop->getDouble());
		} else if (propertyName == PRNL_RENDERER_SOFT_PARTICLES_DELTA) {
			// Soft particles delta: ParticleUniverse::Real
			renderer->setSoftParticlesDelta(prop->getDouble());
		} else {
			// Update renderer with another attribute
			RendererPropertyWindow::copyAttributeToRenderer(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
