#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_POSITION_EMITTER_POSITION = "";
	static QString PRNL_POSITION_EMITTER_RANDOMIZE = "";

	class PositionEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		PositionEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~PositionEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_POSITIONEMITTERPROPERTYWINDOW_H__ */
