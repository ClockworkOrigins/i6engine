#include "widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"

#include "ParticleObservers/ParticleUniverseOnRandomObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnRandomObserverPropertyWindow::OnRandomObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_RANDOM_VALUE = "Random threshold";

		// Random Threshold: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ON_RANDOM_VALUE, PRNL_ON_RANDOM_VALUE, ParticleUniverse::OnRandomObserver::DEFAULT_THRESHOLD));
	}

	OnRandomObserverPropertyWindow::~OnRandomObserverPropertyWindow() {
	}

	void OnRandomObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnRandomObserver * onRandomObserver = static_cast<ParticleUniverse::OnRandomObserver *>(observer);

		// Random Threshold
		setDouble(PRNL_ON_RANDOM_VALUE, onRandomObserver->getThreshold());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
