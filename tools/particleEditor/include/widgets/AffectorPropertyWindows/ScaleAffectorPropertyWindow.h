#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_XYZ_SCALE = "";
	static QString PRNL_X_SCALE = "";
	static QString PRNL_Y_SCALE = "";
	static QString PRNL_Z_SCALE = "";

	class ScaleAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ScaleAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~ScaleAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;

	private:
		void copyAttributeToAffector(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SCALEAFFECTORPROPERTYWINDOW_H__ */
