#include "widgets/RendererPropertyWindows/EntityRendererPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/StringProperty.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	EntityRendererPropertyWindow::EntityRendererPropertyWindow(QWidget * par, QString name) : RendererPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_RENDERER_TYPE = "Renderer type";
		PRNL_ORIENTATION_TYPE = "Orientation type";
		OTT_ORIENTED_SELF = "Oriented self";
		OTT_ORIENTED_SELF_MIRRORED = "Oriented self mirrored";
		OTT_ORIENTED_SHAPE = "Oriented shape";
		PRNL_MESH_NAME = "Mesh name";

		// Mesh Name: Ogre::String
		append(new properties::StringProperty(this, PRNL_MESH_NAME, PRNL_MESH_NAME, ""));

		// Orientation Type: List
		QStringList orientationTypes;
		orientationTypes.append(OTT_ORIENTED_SELF);
		orientationTypes.append(OTT_ORIENTED_SELF_MIRRORED);
		orientationTypes.append(OTT_ORIENTED_SHAPE);
		append(new properties::EnumProperty(this, PRNL_ORIENTATION_TYPE, PRNL_ORIENTATION_TYPE, orientationTypes));
	}

	EntityRendererPropertyWindow::~EntityRendererPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
