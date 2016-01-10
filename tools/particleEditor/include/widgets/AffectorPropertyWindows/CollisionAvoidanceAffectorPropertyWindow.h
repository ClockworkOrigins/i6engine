#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_COLLISON_AVOIDANCE_RADIUS = "";

	class CollisionAvoidanceAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		CollisionAvoidanceAffectorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~CollisionAvoidanceAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;

	private:
		void copyAttributeToAffector(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLLISIONAVOIDANCEAFFECTORPROPERTYWINDOW_H__ */
