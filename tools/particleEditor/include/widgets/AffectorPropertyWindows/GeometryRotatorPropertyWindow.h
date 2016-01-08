#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class GeometryRotatorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		GeometryRotatorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~GeometryRotatorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__ */
