#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class GravityExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GravityExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~GravityExternPropertyWindow();

		void copyAttributesFromExtern(ParticleUniverse::Extern * externObject) override;

	private:
		void copyAttributeToExtern(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYEXTERNPROPERTYWINDOW_H__ */
