#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_LINE_END = "";
	static QString PRNL_LINE_MAX_INCREMENT = "";
	static QString PRNL_LINE_MIN_INCREMENT = "";
	static QString PRNL_LINE_DEVIATION = "";

	class LineEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		LineEmitterPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~LineEmitterPropertyWindow();

		void copyAttributesFromEmitter(ParticleUniverse::ParticleEmitter * emitter) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_LINEEMITTERPROPERTYWINDOW_H__ */
