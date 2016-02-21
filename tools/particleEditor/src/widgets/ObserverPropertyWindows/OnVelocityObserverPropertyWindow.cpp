#include "widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleObservers/ParticleUniverseOnVelocityObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnVelocityObserverPropertyWindow::OnVelocityObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_VELOCITY_COMPARE = QApplication::tr("Velocity compare");
		PRNL_ON_VELOCITY_THRESHOLD = QApplication::tr("Velocity threshold");
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

	void OnVelocityObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnVelocityObserver * onVelocityObserver = static_cast<ParticleUniverse::OnVelocityObserver *>(observer);

		// Velocity threshold - compare: List
		ParticleUniverse::ComparisionOperator compare = onVelocityObserver->getCompare();
		QString compareString = PRNL_COMPARE_LESS_THAN;
		if (compare == ParticleUniverse::CO_GREATER_THAN) {
			compareString = PRNL_COMPARE_GREATER_THAN;
		} else if (compare == ParticleUniverse::CO_EQUALS) {
			compareString = PRNL_COMPARE_EQUALS;
		}
		setEnumString(PRNL_ON_VELOCITY_COMPARE, compareString);

		// Velocity threshold - value: ParticleUniverse::Real
		setDouble(PRNL_ON_VELOCITY_THRESHOLD, onVelocityObserver->getThreshold());
	}

	void OnVelocityObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::OnVelocityObserver * observer = static_cast<ParticleUniverse::OnVelocityObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_ON_VELOCITY_COMPARE) {
			// Velocity threshold - compare: List
			QString compare = prop->getEnumString();
			if (compare == PRNL_COMPARE_LESS_THAN) {
				observer->setCompare(ParticleUniverse::CO_LESS_THAN);
			} else if (compare == PRNL_COMPARE_GREATER_THAN) {
				observer->setCompare(ParticleUniverse::CO_GREATER_THAN);
			} else if (compare == PRNL_COMPARE_EQUALS) {
				observer->setCompare(ParticleUniverse::CO_EQUALS);
			}
		} else if (propertyName == PRNL_ON_VELOCITY_THRESHOLD) {
			// Velocity threshold - value: ParticleUniverse::Real
			observer->setThreshold(prop->getDouble());
		} else {
			// Update observer with another attribute
			ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
