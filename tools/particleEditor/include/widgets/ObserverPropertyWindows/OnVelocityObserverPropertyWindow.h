#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONVELOCITYOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONVELOCITYOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ON_VELOCITY_COMPARE = "";
	static QString PRNL_ON_VELOCITY_THRESHOLD = "";

	class OnVelocityObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnVelocityObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnVelocityObserverPropertyWindow();

		void copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) override;

	private:
		void copyAttributeToObserver(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONVELOCITYOBSERVERPROPERTYWINDOW_H__ */
