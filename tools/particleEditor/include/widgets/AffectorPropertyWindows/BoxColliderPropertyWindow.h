#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_BOX_COLLIDER_WIDTH = "";
	static QString PRNL_BOX_COLLIDER_HEIGHT = "";
	static QString PRNL_BOX_COLLIDER_DEPTH = "";
	static QString PRNL_BOX_COLLIDER_INNER = "";

	class BoxColliderPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BoxColliderPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~BoxColliderPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BOXCOLLIDERPROPERTYWINDOW_H__ */
