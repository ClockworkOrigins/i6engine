#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	class SphereColliderExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SphereColliderExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~SphereColliderExternPropertyWindow();

		void copyAttributesFromExtern(ParticleUniverse::Extern * externObject) override;

	private:
		void copyAttributeToExtern(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__ */
