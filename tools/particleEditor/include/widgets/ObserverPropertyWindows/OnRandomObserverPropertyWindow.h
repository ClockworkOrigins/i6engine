#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ON_RANDOM_VALUE = "";

	class OnRandomObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnRandomObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnRandomObserverPropertyWindow();

		void copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONRANDOMOBSERVERPROPERTYWINDOW_H__ */
