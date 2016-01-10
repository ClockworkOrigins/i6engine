#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"

#include "properties/Vec3ListProperty.h"

#include "ParticleAffectors/ParticleUniversePathFollower.h"

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

	void PathFollowerPropertyWindow::copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) {
		AffectorPropertyWindow::copyAttributesFromAffector(affector);

		// Copy properties from affector to property window
		ParticleUniverse::PathFollower * pathFollower = static_cast<ParticleUniverse::PathFollower *>(affector);

		// Positions: List
		unsigned short numPositions = pathFollower->getNumPoints();
		std::vector<Ogre::Vector3> positions;
		for (unsigned short i = 0; i < numPositions; ++i) {
			positions.push_back(pathFollower->getPoint(i));
		}
		setVector3List(PRNL_PATHFOLLOWER_POSITION, positions);
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
