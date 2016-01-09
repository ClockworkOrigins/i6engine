#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEAMRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BEAMRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_UPDATE_INTERVAL = "";
	static QString PRNL_DEVIATION = "";
	static QString PRNL_NUMBER_OF_SEGMENTS = "";
	static QString PRNL_JUMP = "";
	static QString PRNL_TEXTURE_DIRECTION = "";
	static QString TEXTURE_DIRECTION_U = "";
	static QString TEXTURE_DIRECTION_V = "";

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
