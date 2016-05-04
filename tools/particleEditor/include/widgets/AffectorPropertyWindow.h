#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleAffector;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {
	
	static QString PRNL_AFFECTOR_TYPE = "";
	static QString PRNL_AFFECTOR_ENABLED = "";
	static QString PRNL_AFFECTOR_POSITION = "";
	static QString PRNL_AFFECTOR_MASS = "";
	static QString PRNL_AFFECTOR_SPECIALISATION = "";
	static QString PRNL_AFFECTOR_SPEC_DEFAULT = "";
	static QString PRNL_AFFECTOR_SPEC_TT_INCREASE = "";
	static QString PRNL_AFFECTOR_SPEC_TT_DECREASE = "";

	class AffectorPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		AffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~AffectorPropertyWindow();

		virtual void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector);

		void changedProperty(properties::Property * prop, QString name) override;

	protected:
		virtual void copyAttributeToAffector(properties::Property * prop, QString propertyName);

	private:
		void replaceAffectorType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__ */
