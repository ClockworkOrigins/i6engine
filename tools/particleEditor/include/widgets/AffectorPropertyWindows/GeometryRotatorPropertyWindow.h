#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__

#include "widgets/AffectorPropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_USE_OWN_ROTATION = "";

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

		void copyAttributesFromAffector(ParticleUniverse::ParticleAffector * affector) override;
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_GEOMETRYROTATORPROPERTYWINDOW_H__ */
