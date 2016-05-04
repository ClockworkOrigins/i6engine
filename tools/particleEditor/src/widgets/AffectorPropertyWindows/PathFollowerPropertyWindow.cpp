#include "widgets/AffectorPropertyWindows/PathFollowerPropertyWindow.h"

#include "properties/Vec3ListProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleAffectors/ParticleUniversePathFollower.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	PathFollowerPropertyWindow::PathFollowerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : AffectorPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_PATHFOLLOWER_POSITION = QApplication::tr("Add Position");
		PRNL_POSITION = QApplication::tr("Position");

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

	void PathFollowerPropertyWindow::copyAttributeToAffector(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::PathFollower * affector = static_cast<ParticleUniverse::PathFollower *>(_owner->getPUElement());
		if (!affector) {
			return;
		}

		if (propertyName == PRNL_PATHFOLLOWER_POSITION) {
			// Positions: List
			affector->clearPoints();
			for (ParticleUniverse::Vector3 vec : prop->getVector3List()) {
				affector->addPoint(vec);
			}
		} else {
			// Update affector with another attribute
			AffectorPropertyWindow::copyAttributeToAffector(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
