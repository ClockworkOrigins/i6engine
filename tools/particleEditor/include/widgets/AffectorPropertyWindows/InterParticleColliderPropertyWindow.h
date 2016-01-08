#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_INTERPARTICLECOLLIDERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_INTERPARTICLECOLLIDERPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

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
