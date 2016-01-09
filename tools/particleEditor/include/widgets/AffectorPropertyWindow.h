#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__

#include "widgets/PropertyWindow.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {
	
	static QString PRNL_AFFECTOR_TYPE = "";
	static QString PRNL_AFFECTOR_ENABLED = "";
	static QString PRNL_AFFECTOR_POSITION = "";
	static QString PRNL_AFFECTOR_MASS = "";
	static QString PRNL_AFFECTOR_SPECIALISATION = "";
	static QString PRNL_AFFECTOR_SPEC_DEFAULT = "";
	static QString PRNL_AFFECTOR_SPEC_TT_INCREASE = "";
	static QString PRNL_AFFECTOR_SPEC_TT_DECREASE = "";

	class AffectorPropertyWindow : public PropertyWindow {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		AffectorPropertyWindow(QWidget * par, QString name = "");

		/**
		 * \brief destructor
		 */
		virtual ~AffectorPropertyWindow();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_AFFECTORPROPERTYWINDOW_H__ */
