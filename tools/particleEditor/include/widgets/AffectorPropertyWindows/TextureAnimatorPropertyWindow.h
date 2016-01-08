#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class TextureAnimatorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TextureAnimatorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~TextureAnimatorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__ */
