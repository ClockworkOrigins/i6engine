#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class ParticleRenderer;
} /* namespace ParticleUniverse */

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_RENDERER_TYPE = "";
	static QString PRNL_RENDERER_RENDER_Q_GROUP = "";
	static QString PRNL_RENDERER_SORTING = "";
	static QString PRNL_RENDERER_TEXCOORDS_DEFINE = "";
	static QString PRNL_RENDERER_TEXCOORDS_SET = "";
	static QString PRNL_RENDERER_TEXCOORDS_ROWS = "";
	static QString PRNL_RENDERER_TEXCOORDS_COLUMNS = "";
	static QString PRNL_RENDERER_USE_SOFT_PARTICLES = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_CONTRAST_POWER = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_SCALE = "";
	static QString PRNL_RENDERER_SOFT_PARTICLES_DELTA = "";
	static QString PRNL_USE_VERTEX_COLOURS = "";
	static QString PRNL_MAX_ELEMENTS = "";

	class RendererPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		RendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~RendererPropertyWindow();

		virtual void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer);

	protected:
		void changedProperty(properties::Property * prop, QString name) override;
		virtual void copyAttributeToRenderer(properties::Property * prop, QString propertyName);
		void replaceRendererType(properties::Property * prop);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__ */
