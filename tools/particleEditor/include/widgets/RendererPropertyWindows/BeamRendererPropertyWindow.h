#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEAMRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEAMRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class BeamRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BeamRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~BeamRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEAMRENDERERPROPERTYWINDOW_H__ */
