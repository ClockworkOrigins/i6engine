#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_GRAVITY = "";

	class GravityAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GravityAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~GravityAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GRAVITYAFFECTORPROPERTYWINDOW_H__ */
