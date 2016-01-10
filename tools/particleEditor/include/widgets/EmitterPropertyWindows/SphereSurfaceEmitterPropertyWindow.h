#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERESURFACEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERESURFACEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_SPHERE_SURFACE_EMITTER_RADIUS = "";

	class SphereSurfaceEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		SphereSurfaceEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~SphereSurfaceEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_SPHERESURFACEEMITTERPROPERTYWINDOW_H__ */
