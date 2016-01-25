#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_JETAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_JETAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ACCELERATION = "";

	class JetAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		JetAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~JetAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;

	private:
		void copyAttributeToAffector(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_JETAFFECTORPROPERTYWINDOW_H__ */