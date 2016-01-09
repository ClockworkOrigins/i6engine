#include "widgets/RendererPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleUniverseRenderer.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	RendererPropertyWindow::RendererPropertyWindow(QWidget * par, QString name) : PropertyWindow(par, name) {
		// Set the (internationalized) property names
		// Renderers
		CST_RENDERER_BEAM = RENDERER_BEAM;
		CST_RENDERER_BILLBOARD = RENDERER_BILLBOARD;
		CST_RENDERER_SPHERE = RENDERER_SPHERE;
		CST_RENDERER_BOX = RENDERER_BOX;
		CST_RENDERER_ENTITY = RENDERER_ENTITY;
		CST_RENDERER_LIGHT = RENDERER_LIGHT;
		CST_RENDERER_RIBBONTRAIL = RENDERER_RIBBONTRAIL;
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_USE_VERTEX_COLOURS = "Use vertex colours";
		PRNL_MAX_ELEMENTS = "Maximum elements";
		PRNL_RENDERER_RENDER_Q_GROUP = "Render queue group";
		PRNL_RENDERER_SORTING = "Sorting";
		PRNL_RENDERER_TEXCOORDS_DEFINE = "Texture coords define";
		PRNL_RENDERER_TEXCOORDS_SET = "Texture coords set";
		PRNL_RENDERER_TEXCOORDS_ROWS = "Texture coords rows";
		PRNL_RENDERER_TEXCOORDS_COLUMNS = "Texture coords columns";
		PRNL_RENDERER_USE_SOFT_PARTICLES = "Use soft particles";
		PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = "Soft particles contrast power";
		PRNL_RENDERER_SOFT_PARTICLES_SCALE = "Soft particles scale";
		PRNL_RENDERER_SOFT_PARTICLES_DELTA = "Soft particles delta";
		PRNL_NAME = "Name";
		PRNL_MESH_NAME = "Mesh name";

		// Type: List of types (DO NOT CHANGE THE ORDER!
		// Internationalization means that the string values of this property are unusable to create a renderer)
		QStringList types;
		types.append(CST_RENDERER_BILLBOARD);
		types.append(CST_RENDERER_BEAM);
		types.append(CST_RENDERER_BOX);
		types.append(CST_RENDERER_ENTITY);
		types.append(CST_RENDERER_LIGHT);
		types.append(CST_RENDERER_RIBBONTRAIL);
		types.append(CST_RENDERER_SPHERE);
		append(new properties::EnumProperty(this, PRNL_RENDERER_TYPE, PRNL_RENDERER_TYPE, types));

		// Render queue group: ParticleUniverse::uint8
		append(new properties::UIntProperty(this, PRNL_RENDERER_RENDER_Q_GROUP, PRNL_RENDERER_RENDER_Q_GROUP, ParticleUniverse::ParticleRenderer::DEFAULT_RENDER_QUEUE_GROUP));

		// Sorting: Bool
		append(new properties::BoolProperty(this, PRNL_RENDERER_SORTING, PRNL_RENDERER_SORTING, ParticleUniverse::ParticleRenderer::DEFAULT_SORTED));

		// Texture coords define: Is no attribute, but only a 'container' in the script

		// Texture coords set: List of 4 x ParticleUniverse::Real
		// TODO

		// Texture coords rows: uchar
		append(new properties::UIntProperty(this, PRNL_RENDERER_TEXCOORDS_ROWS, PRNL_RENDERER_TEXCOORDS_ROWS, ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_ROWS));

		// Texture coords columns: uchar
		append(new properties::UIntProperty(this, PRNL_RENDERER_TEXCOORDS_COLUMNS, PRNL_RENDERER_TEXCOORDS_COLUMNS, ParticleUniverse::ParticleRenderer::DEFAULT_TEXTURECOORDS_COLUMNS));
	}

	RendererPropertyWindow::~RendererPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
