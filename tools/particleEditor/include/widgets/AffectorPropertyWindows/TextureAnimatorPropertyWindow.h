#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ANIMATION_TYPE = "";
	static QString TAT_LOOP = "";
	static QString TAT_UP_DOWN = "";
	static QString TAT_RANDOM = "";
	static QString PRNL_TEXCOORDS_START = "";
	static QString PRNL_TEXCOORDS_END = "";
	static QString PRNL_START_RANDOM = "";

	class TextureAnimatorPropertyWindow : public AffectorPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		TextureAnimatorPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~TextureAnimatorPropertyWindow();

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_TEXTUREANIMATORPROPERTYWINDOW_H__ */
