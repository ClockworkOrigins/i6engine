#include "widgets/ObserverPropertyWindows/OnTimeObserverPropertyWindow.h"

#include "properties/BoolProperty.h"
#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "ParticleObservers/ParticleUniverseOnTimeObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnTimeObserverPropertyWindow::OnTimeObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_TIME_COMPARE = "On time compare";
		PRNL_ON_TIME_THRESHOLD = "On time threshold";
		PRNL_SINCE_START_SYSTEM = "Since start system";
		PRNL_COMPARE_LESS_THAN = "Less than";
		PRNL_COMPARE_GREATER_THAN = "Greater than";
		PRNL_COMPARE_EQUALS = "Equals";

		// Time Threshold - Compare: List
		QStringList compare;
		compare.append(PRNL_COMPARE_LESS_THAN);
		compare.append(PRNL_COMPARE_GREATER_THAN);
		compare.append(PRNL_COMPARE_EQUALS);
		append(new properties::EnumProperty(this, PRNL_ON_TIME_COMPARE, PRNL_ON_TIME_COMPARE, compare));

		// Time Threshold - Value: ParticleUniverse::Real
		append(new properties::DoubleProperty(this, PRNL_ON_TIME_THRESHOLD, PRNL_ON_TIME_THRESHOLD, ParticleUniverse::OnTimeObserver::DEFAULT_THRESHOLD));

		// Since Start System: bool
		append(new properties::BoolProperty(this, PRNL_SINCE_START_SYSTEM, PRNL_SINCE_START_SYSTEM, ParticleUniverse::OnTimeObserver::DEFAULT_SINCE_START_SYSTEM));
	}

	OnTimeObserverPropertyWindow::~OnTimeObserverPropertyWindow() {
	}

	void OnTimeObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnTimeObserver * onTimeObserver = static_cast<ParticleUniverse::OnTimeObserver *>(observer);

		// Time Threshold - Compare: List
		ParticleUniverse::ComparisionOperator compare = onTimeObserver->getCompare();
		QString compareString = PRNL_COMPARE_LESS_THAN;
		if (compare == ParticleUniverse::CO_GREATER_THAN) {
			compareString = PRNL_COMPARE_GREATER_THAN;
		} else if (compare == ParticleUniverse::CO_EQUALS) {
			compareString = PRNL_COMPARE_EQUALS;
		}
		setEnumString(PRNL_ON_TIME_COMPARE, compareString);

		// Time Threshold - Value: ParticleUniverse::Real
		setDouble(PRNL_ON_TIME_THRESHOLD, onTimeObserver->getThreshold());

		// Since Start System: bool
		setBool(PRNL_SINCE_START_SYSTEM, onTimeObserver->isSinceStartSystem());
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
