#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__

#include "widgets/EmitterPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class MeshSurfaceEmitterPropertyWindow : public EmitterPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		MeshSurfaceEmitterPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~MeshSurfaceEmitterPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_MESHSURFACEEMITTERPROPERTYWINDOW_H__ */
