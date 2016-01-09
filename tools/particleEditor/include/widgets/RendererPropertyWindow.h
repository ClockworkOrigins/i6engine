#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
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
		RendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~RendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_RENDERERPROPERTYWINDOW_H__ */
