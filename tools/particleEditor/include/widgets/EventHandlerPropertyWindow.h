#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleEventHandler;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_HANDLER_TYPE = "";

	class EventHandlerPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EventHandlerPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~EventHandlerPropertyWindow();

		virtual void copyAttributesFromEventHandler(ParticleUniverse::ParticleEventHandler * eventHandler);

	protected:
		void changedProperty(properties::Property * prop, QString name) override;
		virtual void copyAttributeToEventHandler(properties::Property * prop, QString propertyName);

	private:
		void replaceHandlerType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EVENTHANDLERPROPERTYWINDOW_H__ */
