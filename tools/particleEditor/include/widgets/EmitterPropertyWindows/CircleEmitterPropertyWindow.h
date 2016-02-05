#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_CIRCLE_EMITTER_RADIUS = "";
	static QString PRNL_CIRCLE_EMITTER_STEP = "";
	static QString PRNL_CIRCLE_EMITTER_ANGLE = "";
	static QString PRNL_CIRCLE_EMITTER_RANDOM = "";
	static QString PRNL_CIRCLE_EMITTER_NORMAL = "";

	class CircleEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		CircleEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~CircleEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;

	private:
		void copyAttributeToEmitter(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_CIRCLEEMITTERPROPERTYWINDOW_H__ */
