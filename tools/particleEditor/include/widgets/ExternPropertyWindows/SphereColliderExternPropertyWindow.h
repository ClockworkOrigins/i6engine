#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__

#include "widgets/ExternPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class SphereColliderExternPropertyWindow : public ExternPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SphereColliderExternPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~SphereColliderExternPropertyWindow();

		void copyAttributesFromExtern(ParticleUniverse::Extern * externObject) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERECOLLIDEREXTERNPROPERTYWINDOW_H__ */
