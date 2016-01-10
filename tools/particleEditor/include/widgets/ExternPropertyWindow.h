#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_EXTERNPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_EXTERNPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace ParticleUniverse {
	class Extern;
} /* namespace ParticleUniverse */

namespace i6engine {
namespace particleEditor {
namespace widgets {

	// Property names
	static QString PRNL_EXTERN_TYPE = "";

	class ExternPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		ExternPropertyWindow(QWidget * par, WidgetEditComponent * owner, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~ExternPropertyWindow();

		virtual void copyAttributesFromExtern(ParticleUniverse::Extern * externObject);
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_EXTERNPROPERTYWINDOW_H__ */
