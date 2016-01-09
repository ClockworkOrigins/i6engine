#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"

#include "properties/Vec3ListProperty.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	PathFollowerPropertyWindow::PathFollowerPropertyWindow(QWidget * par, QString name) : AffectorPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_PATHFOLLOWER_POSITION = "Add Position";
		PRNL_POSITION = "Position";
		PRNL_POSITION_X = ".x";
		PRNL_POSITION_Y = ".y";
		PRNL_POSITION_Z = ".z";

		// Positions
		append(new properties::Vec3ListProperty(this, PRNL_PATHFOLLOWER_POSITION, PRNL_PATHFOLLOWER_POSITION));
	}

	PathFollowerPropertyWindow::~PathFollowerPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
