#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/EventHandlerPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_SCALE_FRACTION = "";
	static QString PRNL_SCALE_TYPE = "";
	static QString SC_TIME_TO_LIVE = "";
	static QString SC_VELOCITY = "";

	class DoScaleEventHandlerPropertyWindow : public EventHandlerPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DoScaleEventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~DoScaleEventHandlerPropertyWindow();

		void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler) override;

	private:
		void copyAttributeToEventHandler(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_DOSCALEEVENTHANDLERPROPERTYWINDOW_H__ */
