#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREROTATORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREROTATORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class TextureRotatorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TextureRotatorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~TextureRotatorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREROTATORPROPERTYWINDOW_H__ */
