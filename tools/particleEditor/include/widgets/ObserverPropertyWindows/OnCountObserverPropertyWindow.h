#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__

#include "widgets/ObserverPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ON_COUNT_COMPARE = "";
	static QString PRNL_ON_COUNT_THRESHOLD = "";

	class OnCountObserverPropertyWindow : public ObserverPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		OnCountObserverPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~OnCountObserverPropertyWindow();

		void copyAttributesFromObserver(ParticleUniverse::ParticleObserver * observer) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ONCOUNTOBSERVERPROPERTYWINDOW_H__ */
