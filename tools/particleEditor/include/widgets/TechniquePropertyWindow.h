#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class TechniquePropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TechniquePropertyWindow(QWidget * par, QString name);

		/**
		 * \brief destructor
		 */
		~TechniquePropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_TECHNIQUEPROPERTYWINDOW_H__ */
