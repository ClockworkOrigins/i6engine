#include "widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.h"

#include "properties/EnumProperty.h"
#include "properties/UIntProperty.h"

#include "ParticleObservers/ParticleUniverseOnCountObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnCountObserverPropertyWindow::OnCountObserverPropertyWindow(QWidget * par, QString name) : ObserverPropertyWindow(par, name) {
		// Set the (internationalized) property names
		PRNL_ON_COUNT_COMPARE = "On count compare";
		PRNL_ON_COUNT_THRESHOLD = "On count threshold";
		PRNL_COMPARE_LESS_THAN = "Less than";
		PRNL_COMPARE_GREATER_THAN = "Greater than";
		PRNL_COMPARE_EQUALS = "Equals";

		// Count Threshold Compare: List
		QStringList compare;
		compare.append(PRNL_COMPARE_LESS_THAN);
		compare.append(PRNL_COMPARE_GREATER_THAN);
		compare.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_COUNT_COMPARE, PRNL_ON_COUNT_COMPARE, compare));

		// Count Threshold Value: ParticleUniverse::uint
		append(new properties::UIntProperty(this, PRNL_ON_COUNT_THRESHOLD, PRNL_ON_COUNT_THRESHOLD, ParticleUniverse::OnCountObserver::DEFAULT_THRESHOLD));
	}

	OnCountObserverPropertyWindow::~OnCountObserverPropertyWindow() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
