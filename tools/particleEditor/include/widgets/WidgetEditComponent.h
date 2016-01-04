#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__

#include "ui_widgetEditComponent.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class WidgetEditComponent : public QWidget, public Ui::editComponentWidget {
		Q_OBJECT

	public:
		static QString EC_SYSTEM;
		static QString EC_RENDERER;
		static QString EC_TECHNIQUE;
		static QString EC_EMITTER;
		static QString EC_AFFECTOR;
		static QString EC_OBSERVER;
		static QString EC_HANDLER;
		static QString EC_BEHAVIOUR;
		static QString EC_EXTERN;

		/**
		 * \brief constructor
		 */
		WidgetEditComponent(QWidget * par, QString name, QString type, QString subType);

		/**
		 * \brief destructor
		 */
		~WidgetEditComponent();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETEDITCOMPONENT_H__ */
