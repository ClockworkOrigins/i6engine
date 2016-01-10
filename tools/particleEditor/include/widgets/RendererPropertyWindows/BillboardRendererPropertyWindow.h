#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_BILLBOARD_TYPE;
	static QString BBT_POINT;
	static QString BBT_ORIENTED_COMMON;
	static QString BBT_ORIENTED_SELF;
	static QString BBT_PERPENDICULAR_COMMON;
	static QString BBT_PERPENDICULAR_SELF;
	static QString BBT_ORIENTED_SHAPE;
	static QString PRNL_ORIGIN;
	static QString ORG_TOP_LEFT;
	static QString ORG_TOP_CENTER;
	static QString ORG_TOP_RIGHT;
	static QString ORG_CENTER_LEFT;
	static QString ORG_CENTER_RIGHT;
	static QString ORG_CENTER;
	static QString ORG_BOTTOM_LEFT;
	static QString ORG_BOTTOM_RIGHT;
	static QString ORG_BOTTOM_CENTER;
	static QString PRNL_ROTATION_TYPE;
	static QString RT_VERTEX;
	static QString RT_TEXTURE_COORDINATES;
	static QString PRNL_COMMON_DIRECTION;
	static QString PRNL_UP_VECTOR;
	static QString PRNL_POINT_RENDERING;
	static QString PRNL_ACCURATE_FACING;
	static QString ACF_ON;
	static QString ACF_OFF;

	class BillboardRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		BillboardRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~BillboardRendererPropertyWindow();

		void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_BILLBOARDRENDERERPROPERTYWINDOW_H__ */
