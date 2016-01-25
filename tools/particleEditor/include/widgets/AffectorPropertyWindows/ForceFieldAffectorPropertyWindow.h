#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_AFFECTOR_FORCEFIELD_FF_TYPE = "";
	static QString FORCEFIELD_REALTIME = "";
	static QString FORCEFIELD_MATRIX = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_DELTA = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FORCE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_OCTAVES = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FREQUENCY = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_AMPLITUDE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_PERSISTENCE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_FF_SIZE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_WORLDSIZE = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_X = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_Y = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_IGNORE_Z = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_MOVEMENT = "";
	static QString PRNL_AFFECTOR_FORCEFIELD_MOVE_FREQUENCY = "";

	class ForceFieldAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ForceFieldAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~ForceFieldAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;

	private:
		void copyAttributeToAffector(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_FORCEFIELDAFFECTORPROPERTYWINDOW_H__ */
