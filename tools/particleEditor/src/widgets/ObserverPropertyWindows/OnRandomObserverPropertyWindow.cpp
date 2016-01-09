#include "widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(QWidget * par, QString name) : ObserverPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ON_RANDOM_VALUE = "Random threshold";

		// Random Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ON_RANDOM_VALUE, PRNL_ON_RANDOM_VALUE, ParticleUniverse::OnRandomObserver::DEFAULT_THRESHOLD));
	}

	OnRandomObserverPropertyWindow::~OnRandomObserverPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
