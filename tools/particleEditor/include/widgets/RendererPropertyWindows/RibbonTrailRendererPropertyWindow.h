#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_TRAIL_LENGTH;
	static QString PRNL_TRAIL_WIDTH;
	static QString PRNL_RANDOM_INITIAL_COLOUR;
	static QString PRNL_INITIAL_COLOUR;
	static QString PRNL_COLOUR_CHANGE;

	class RibbonTrailRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RibbonTrailRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~RibbonTrailRendererPropertyWindow();

		void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__ */
