#include "widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnPositionObserverPropertyWindow::OnPositionObserverPropertyWindow(QWidget * par, QString name) : ObserverPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ON_POSITION_X_COMPARE = "Position compare x";
		PRNL_ON_POSITION_Y_COMPARE = "Position compare y";
		PRNL_ON_POSITION_Z_COMPARE = "Position compare z";
		PRNL_ON_POSITION_X = "Position.x threshold";
		PRNL_ON_POSITION_Y = "Position.y threshold";
		PRNL_ON_POSITION_Z = "Position.z threshold";
		PRNL_COMPARE_NOT_USED = "Not used";
		PRNL_COMPARE_LESS_THAN = "Less than";
		PRNL_COMPARE_GREATER_THAN = "Greater than";
		PRNL_COMPARE_EQUALS = "Equals";

		// Compare X
		QStringList compareX;
		compareX.append(PRNL_COMPARE_NOT_USED);
		compareX.append(PRNL_COMPARE_LESS_THAN);
		compareX.append(PRNL_COMPARE_GREATER_THAN);
		compareX.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_POSITION_X_COMPARE, PRNL_ON_POSITION_X_COMPARE, compareX));

		// Position X
		append(new properties::DoubleProperty(this, PRNL_ON_POSITION_X, PRNL_ON_POSITION_X, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.x));

		// Compare Y
		QStringList compareY;
		compareY.append(PRNL_COMPARE_NOT_USED);
		compareY.append(PRNL_COMPARE_LESS_THAN);
		compareY.append(PRNL_COMPARE_GREATER_THAN);
		compareY.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_POSITION_Y_COMPARE, PRNL_ON_POSITION_Y_COMPARE, compareY));

		// Position Y
		append(new properties::DoubleProperty(this, PRNL_ON_POSITION_Y, PRNL_ON_POSITION_Y, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.y));

		// Compare Z
		QStringList compareZ;
		compareZ.append(PRNL_COMPARE_NOT_USED);
		compareZ.append(PRNL_COMPARE_LESS_THAN);
		compareZ.append(PRNL_COMPARE_GREATER_THAN);
		compareZ.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_POSITION_Z_COMPARE, PRNL_ON_POSITION_Z_COMPARE, compareZ));

		// Position Z
		append(new properties::DoubleProperty(this, PRNL_ON_POSITION_Z, PRNL_ON_POSITION_Z, ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.z));
	}

	OnPositionObserverPropertyWindow::~OnPositionObserverPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
