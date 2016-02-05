#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleBehaviour;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_BEHAVIOUR_TYPE = "";

	class BehaviourPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BehaviourPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~BehaviourPropertyWindow();

		virtual void copyAttributesFromBehaviour(ParticleUniverse::ParticleBehaviour * behaviour);

		void changedProperty(properties::Property * prop, QString name) override;

	protected:
		virtual void copyAttributeToBehaviour(properties::Property * prop, QString propertyName);

	private:
		void replaceBehaviourType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEHAVIOURPROPERTYWINDOW_H__ */
