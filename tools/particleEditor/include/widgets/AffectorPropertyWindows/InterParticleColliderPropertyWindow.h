#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_INTERPARTICLECOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_INTERPARTICLECOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_INTERPARTICLE_COLLIDER_ADJUSTMENT = "";
	static QString PRNL_COLLISON_RESPONSE = "";
	static QString CR_AVERAGE_VELOCITY = "";
	static QString CR_ANGLE_BASED_VELOCITY = "";

	class InterParticleColliderPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		InterParticleColliderPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~InterParticleColliderPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_INTERPARTICLECOLLIDERPROPERTYWINDOW_H__ */
