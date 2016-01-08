#include "factories/ObserverPropertyWindowFactory.h"

#include "widgets/WidgetEditComponent.h"

#include "widgets/ObserverPropertyWindows/OnClearObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnCollisionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnCountObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnEmissionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnEventFlagObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnExpireObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnPositionObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnQuotaObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnRandomObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnTimeObserverPropertyWindow.h"
#include "widgets/ObserverPropertyWindows/OnVelocityObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace factories {

	/**
	Create a specific Observer Property Window
	*/
	widgets::ObserverPropertyWindow * ObserverPropertyWindowFactory::createObserverPropertyWindow(QWidget * parent, const QString & name, QString subType) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_CLEAR) {
			return new widgets::OnClearObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_COLLISION) {
			return new widgets::OnCollisionObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_COUNT) {
			return new widgets::OnCountObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EMISSION) {
			return new widgets::OnEmissionObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EVENTFLAG) {
			return new widgets::OnEventFlagObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_EXPIRE) {
			return new widgets::OnExpireObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_POSITION) {
			return new widgets::OnPositionObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_QUOTA) {
			return new widgets::OnQuotaObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_RANDOM) {
			return new widgets::OnRandomObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_TIME) {
			return new widgets::OnTimeObserverPropertyWindow(parent, name);
		} else if (subType == widgets::CST_OBSERVER_ON_VELOCITY) {
			return new widgets::OnVelocityObserverPropertyWindow(parent, name);
		} else {
			return new widgets::ObserverPropertyWindow(parent, name);
		}
		return nullptr;
	}

	/**
	Create a specific Observer Property Window and propagate the attributes from the ObserverPropertyWindow
	*/
	widgets::ObserverPropertyWindow * ObserverPropertyWindowFactory::createObserverPropertyWindow(QString subType, widgets::ObserverPropertyWindow * observerPropertyWindow) {
		if (subType == widgets::CST_UNDEFINED) {
			return new widgets::ObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_CLEAR) {
			return new widgets::OnClearObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_COLLISION) {
			return new widgets::OnCollisionObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_COUNT) {
			return new widgets::OnCountObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_EMISSION) {
			return new widgets::OnEmissionObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_EVENTFLAG) {
			return new widgets::OnEventFlagObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_EXPIRE) {
			return new widgets::OnExpireObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_POSITION) {
			return new widgets::OnPositionObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_QUOTA) {
			return new widgets::OnQuotaObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_RANDOM) {
			return new widgets::OnRandomObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_TIME) {
			return new widgets::OnTimeObserverPropertyWindow(observerPropertyWindow);
		} else if (subType == widgets::CST_OBSERVER_ON_VELOCITY) {
			return new widgets::OnVelocityObserverPropertyWindow(observerPropertyWindow);
		} else {
			return new widgets::ObserverPropertyWindow(observerPropertyWindow);
		}
		return nullptr;
	}

} /* namespace factories */
} /* namespace particleEditor */
} /* namespace i6engine */
