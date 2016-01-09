#include "widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.h"

#include "properties/UIntProperty.h"

#include "ParticleObservers/ParticleUniverseOnEventFlagObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnEventFlagObserverPropertyWindow::OnEventFlagObserverPropertyWindow(QWidget * par, QString name) : ObserverPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ON_EVENTFLAG_FLAG = "Event Flag";

		// Event Flag: ParticleUniverse::uint32
		append(new properties::UIntProperty(this, PRNL_ON_EVENTFLAG_FLAG, PRNL_ON_EVENTFLAG_FLAG, ParticleUniverse::OnEventFlagObserver::DEFAULT_EVENT_FLAG));
	}

	OnEventFlagObserverPropertyWindow::~OnEventFlagObserverPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
