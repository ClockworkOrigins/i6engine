#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_LIGHT_TYPE;
	static QString LT_POINT;
	static QString LT_SPOT;
	static QString PRNL_ATT_RANGE;
	static QString PRNL_ATT_CONSTANT;
	static QString PRNL_ATT_LINEAR;
	static QString PRNL_ATT_QUADRATIC;
	static QString PRNL_SPOT_INNER_ANGLE;
	static QString PRNL_SPOT_OUTER_ANGLE;
	static QString PRNL_FALLOFF;
	static QString PRNL_POWER_SCALE;
	static QString PRNL_FLASH_FREQUENCY;
	static QString PRNL_FLASH_LENGTH;
	static QString PRNL_FLASH_RANDOM;

	class LightRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		LightRendererPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		~LightRendererPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_LIGHTRENDERERPROPERTYWINDOW_H__ */
