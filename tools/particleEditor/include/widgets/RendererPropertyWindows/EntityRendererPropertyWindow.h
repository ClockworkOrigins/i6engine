#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__

#include "widgets/RendererPropertyWindow.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_ORIENTATION_TYPE;
	static QString OTT_ORIENTED_SELF;
	static QString OTT_ORIENTED_SELF_MIRRORED;
	static QString OTT_ORIENTED_SHAPE;

	class EntityRendererPropertyWindow : public RendererPropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		EntityRendererPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		~EntityRendererPropertyWindow();

		void copyAttributesFromRenderer(ParticleUniverse::ParticleRenderer * renderer) override;

	private:
		void copyAttributeToRenderer(properties::Property * prop, QString propertyName) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_ENTITYRENDERERPROPERTYWINDOW_H__ */
