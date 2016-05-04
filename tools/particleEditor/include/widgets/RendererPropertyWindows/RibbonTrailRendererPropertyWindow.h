#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6e {
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
		RibbonTrailRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~RibbonTrailRendererPropertyWindow();

		void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) override;

	private:
		void copyAttributeToRenderer(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RIBBONTRAILRENDERERPROPERTYWINDOW_H__ */
