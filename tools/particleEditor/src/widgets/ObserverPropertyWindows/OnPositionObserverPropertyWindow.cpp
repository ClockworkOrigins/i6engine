#include "widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.h"

#include "properties/DoubleProperty.h"
#include "properties/EnumProperty.h"

#include "widgets/WidgetEditComponent.h"

#include "ParticleObservers/ParticleUniverseOnPositionObserver.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	OnPositionObserverPropertyWindow::OnPositionObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name) : ObserverPropertyWindow(par, owner, name) {
		// Set the (internationalized) property names
		PRNL_ON_POSITION_X_COMPARE = QApplication::tr("Position compare x");
		PRNL_ON_POSITION_Y_COMPARE = QApplication::tr("Position compare y");
		PRNL_ON_POSITION_Z_COMPARE = QApplication::tr("Position compare z");
		PRNL_ON_POSITION_X = QApplication::tr("Position.x threshold");
		PRNL_ON_POSITION_Y = QApplication::tr("Position.y threshold");
		PRNL_ON_POSITION_Z = QApplication::tr("Position.z threshold");
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

	void OnPositionObserverPropertyWindow::copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) {
		ObserverPropertyWindow::copyAttributesFromObserver(observer);

		// Copy properties from observer to property window
		ParticleUniverse::OnPositionObserver * onPositionObserver = static_cast<ParticleUniverse::OnPositionObserver *>(observer);

		// Compare X
		ParticleUniverse::ComparisionOperator compare = onPositionObserver->getComparePositionX();
		QString compareString = PRNL_COMPARE_NOT_USED;
		if (onPositionObserver->isPositionXThresholdSet()) {
			if (compare == ParticleUniverse::CO_LESS_THAN) {
				compareString = PRNL_COMPARE_LESS_THAN;
			} else if (compare == ParticleUniverse::CO_GREATER_THAN) {
				compareString = PRNL_COMPARE_GREATER_THAN;
			} else if (compare == ParticleUniverse::CO_EQUALS) {
				compareString = PRNL_COMPARE_EQUALS;
			}
		}
		setEnumString(PRNL_ON_POSITION_X_COMPARE, compareString);

		// Position X
		setDouble(PRNL_ON_POSITION_X, onPositionObserver->getPositionXThreshold());

		// Compare Y
		compare = onPositionObserver->getComparePositionY();
		compareString = PRNL_COMPARE_NOT_USED;
		if (onPositionObserver->isPositionYThresholdSet()) {
			if (compare == ParticleUniverse::CO_LESS_THAN) {
				compareString = PRNL_COMPARE_LESS_THAN;
			} else if (compare == ParticleUniverse::CO_GREATER_THAN) {
				compareString = PRNL_COMPARE_GREATER_THAN;
			} else if (compare == ParticleUniverse::CO_EQUALS) {
				compareString = PRNL_COMPARE_EQUALS;
			}
		}
		setEnumString(PRNL_ON_POSITION_Y_COMPARE, compareString);

		// Position Y
		setDouble(PRNL_ON_POSITION_Y, onPositionObserver->getPositionYThreshold());

		// Compare Z
		compare = onPositionObserver->getComparePositionZ();
		compareString = PRNL_COMPARE_NOT_USED;
		if (onPositionObserver->isPositionZThresholdSet()) {
			if (compare == ParticleUniverse::CO_LESS_THAN) {
				compareString = PRNL_COMPARE_LESS_THAN;
			} else if (compare == ParticleUniverse::CO_GREATER_THAN) {
				compareString = PRNL_COMPARE_GREATER_THAN;
			} else if (compare == ParticleUniverse::CO_EQUALS) {
				compareString = PRNL_COMPARE_EQUALS;
			}
		}
		setEnumString(PRNL_ON_POSITION_Z_COMPARE, compareString);

		// Position Z
		setDouble(PRNL_ON_POSITION_Z, onPositionObserver->getPositionZThreshold());
	}

	void OnPositionObserverPropertyWindow::copyAttributeToObserver(properties::Property * prop, QString propertyName) {
		if (!prop) {
			return;
		}

		ParticleUniverse::OnPositionObserver * observer = static_cast<ParticleUniverse::OnPositionObserver *>(_owner->getPUElement());
		if (!observer) {
			return;
		}

		if (propertyName == PRNL_ON_POSITION_X_COMPARE) {
			// Compare X: List
			QString compare = prop->getEnumString();
			if (compare == PRNL_COMPARE_NOT_USED) {
				observer->setComparePositionX(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionXThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.x);
				observer->resetPositionXThreshold();
			} else if (compare == PRNL_COMPARE_LESS_THAN) {
				observer->setComparePositionX(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_GREATER_THAN) {
				observer->setComparePositionX(ParticleUniverse::CO_GREATER_THAN);
				observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_EQUALS) {
				observer->setComparePositionX(ParticleUniverse::CO_EQUALS);
				observer->setPositionXThreshold(observer->getPositionXThreshold()); // Forces that compare is used
			}
		} else if (propertyName == PRNL_ON_POSITION_X) {
			// Position X: ParticleUniverse::Real
			observer->setPositionXThreshold(prop->getDouble());

			// Adjust the compare operator
			prop = _properties[PRNL_ON_POSITION_X_COMPARE];
			if (prop) {
				QString compareString = PRNL_COMPARE_LESS_THAN;
				if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN) {
					compareString = PRNL_COMPARE_GREATER_THAN;
				} else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS) {
					compareString = PRNL_COMPARE_EQUALS;
				}
				prop->setEnumString(compareString);
			}
		} else if (propertyName == PRNL_ON_POSITION_Y_COMPARE) {
			// Compare Y: List
			QString compare = prop->getEnumString();
			if (compare == PRNL_COMPARE_NOT_USED) {
				observer->setComparePositionY(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionYThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.y);
				observer->resetPositionYThreshold();
			} else if (compare == PRNL_COMPARE_LESS_THAN) {
				observer->setComparePositionY(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_GREATER_THAN) {
				observer->setComparePositionY(ParticleUniverse::CO_GREATER_THAN);
				observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_EQUALS) {
				observer->setComparePositionY(ParticleUniverse::CO_EQUALS);
				observer->setPositionYThreshold(observer->getPositionYThreshold()); // Forces that compare is used
			}
		} else if (propertyName == PRNL_ON_POSITION_Y) {
			// Position Y: ParticleUniverse::Real
			observer->setPositionYThreshold(prop->getDouble());

			// Adjust the compare operator
			prop = _properties[PRNL_ON_POSITION_Y_COMPARE];
			if (prop) {
				QString compareString = PRNL_COMPARE_LESS_THAN;
				if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN) {
					compareString = PRNL_COMPARE_GREATER_THAN;
				} else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS) {
					compareString = PRNL_COMPARE_EQUALS;
				}
				prop->setEnumString(compareString);
			}
		} else if (propertyName == PRNL_ON_POSITION_Z_COMPARE) {
			// Compare Z: List
			QString compare = prop->getEnumString();
			if (compare == PRNL_COMPARE_NOT_USED) {
				observer->setComparePositionZ(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionZThreshold(ParticleUniverse::OnPositionObserver::DEFAULT_POSITION_THRESHOLD.z);
				observer->resetPositionZThreshold();
			} else if (compare == PRNL_COMPARE_LESS_THAN) {
				observer->setComparePositionZ(ParticleUniverse::CO_LESS_THAN);
				observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_GREATER_THAN) {
				observer->setComparePositionZ(ParticleUniverse::CO_GREATER_THAN);
				observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
			} else if (compare == PRNL_COMPARE_EQUALS) {
				observer->setComparePositionZ(ParticleUniverse::CO_EQUALS);
				observer->setPositionZThreshold(observer->getPositionZThreshold()); // Forces that compare is used
			}
		} else if (propertyName == PRNL_ON_POSITION_Z) {
			// Position Z: ParticleUniverse::Real
			observer->setPositionZThreshold(prop->getDouble());

			// Adjust the compare operator
			prop = _properties[PRNL_ON_POSITION_Z_COMPARE];
			if (prop) {
				QString compareString = PRNL_COMPARE_LESS_THAN;
				if (observer->getComparePositionX() == ParticleUniverse::CO_GREATER_THAN) {
					compareString = PRNL_COMPARE_GREATER_THAN;
				} else if (observer->getComparePositionX() == ParticleUniverse::CO_EQUALS) {
					compareString = PRNL_COMPARE_EQUALS;
				}
				prop->setEnumString(compareString);
			}
		} else {
			// Update observer with another attribute
			ObserverPropertyWindow::copyAttributeToObserver(prop, propertyName);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
