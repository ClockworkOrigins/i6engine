#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class VertexEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		VertexEmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~VertexEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_VERTEXEMITTERPROPERTYWINDOW_H__ */
