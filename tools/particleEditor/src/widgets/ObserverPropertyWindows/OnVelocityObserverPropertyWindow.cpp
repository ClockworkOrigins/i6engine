#include "widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleObservers/ParticleUniverseOnVelocityObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnVelocityObserverPropertyWindow::OnVelocityObserverPropertyWindow(QWidget * par, QString name) : ObserverPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ON_VELOCITY_COMPARE = "Velocity compare";
		PRNL_ON_VELOCITY_THRESHOLD = "Velocity threshold";
		PRNL_COMPARE_LESS_THAN = "Less than";
		PRNL_COMPARE_GREATER_THAN = "Greater than";
		PRNL_COMPARE_EQUALS = "Equals";

		// Velocity threshold - compare: List
		QStringList compare;
		compare.append(PRNL_COMPARE_LESS_THAN);
		compare.append(PRNL_COMPARE_GREATER_THAN);
		compare.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_VELOCITY_COMPARE, PRNL_ON_VELOCITY_COMPARE, compare));

		// Velocity threshold - value: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ON_VELOCITY_THRESHOLD, PRNL_ON_VELOCITY_THRESHOLD, ParticleUniverse::OnVelocityObserver::DEFAULT_VELOCITY_THRESHOLD));
	}

	OnVelocityObserverPropertyWindow::~OnVelocityObserverPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
