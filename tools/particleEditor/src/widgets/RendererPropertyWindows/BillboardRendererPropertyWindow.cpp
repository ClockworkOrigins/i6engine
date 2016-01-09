#include "widgets/RendererPropertyWindows/BillboardRendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"
#include "properties/Vec3Property.h"

#include "ParticleRenderers/ParticleUniverseBillboardRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	BillboardRendererPropertyWindow::BillboardRendererPropertyWindow(QWidget * par, QString name) : RendererPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_BILLBOARD_TYPE = "Billboard type";
		BBT_POINT = "Point";
		BBT_ORIENTED_COMMON = "Oriented Common";
		BBT_ORIENTED_SELF = "Oriented Self";
		BBT_PERPENDICULAR_COMMON = "Perpendicular Common";
		BBT_PERPENDICULAR_SELF = "Perpendicular Self";
		BBT_ORIENTED_SHAPE = "Oriented Shape";
		PRNL_ORIGIN = "Origin";
		ORG_TOP_LEFT = "Top-left";
		ORG_TOP_CENTER = "Top-center";
		ORG_TOP_RIGHT = "Top-right";
		ORG_CENTER_LEFT = "Center-left";
		ORG_CENTER_RIGHT = "Center-right";
		ORG_CENTER = "Center";
		ORG_BOTTOM_LEFT = "Bottom-left";
		ORG_BOTTOM_RIGHT = "Bottom-right";
		ORG_BOTTOM_CENTER = "Bottom-center";
		PRNL_ROTATION_TYPE = "Rotation type";
		RT_VERTEX = "Vertex";
		RT_TEXTURE_COORDINATES = "Texture coordinates";
		PRNL_COMMON_DIRECTION = "Common direction";
		PRNL_UP_VECTOR = "Up Vector";
		PRNL_POINT_RENDERING = "Point Rendering";
		PRNL_ACCURATE_FACING = "Accurate Facing";
		ACF_ON = "On";
		ACF_OFF = "Off";
		PRNL_RENDERER_USE_SOFT_PARTICLES = "Use soft particles";
		PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = "Soft particles contrast power";
		PRNL_RENDERER_SOFT_PARTICLES_SCALE = "Soft particles scale";
		PRNL_RENDERER_SOFT_PARTICLES_DELTA = "Soft particles delta";
		PRNL_X = ".x";
		PRNL_Y = ".y";
		PRNL_Z = ".z";

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

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
