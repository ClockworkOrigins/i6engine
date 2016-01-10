#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_TIME_AND_COLOUR = "";
	static QString PRNL_TIME_AND_COLOUR_PARENT = "";
	static QString PRNL_TIME_AND_COLOUR_TIME = "";
	static QString PRNL_TIME_AND_COLOUR_COLOUR = "";
	static QString PRNL_COLOUR_OPERATION = "";
	static QString COP_SET = "";
	static QString COP_MULTIPLY = "";

	class ColourAffectorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ColourAffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~ColourAffectorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_COLOURAFFECTORPROPERTYWINDOW_H__ */
