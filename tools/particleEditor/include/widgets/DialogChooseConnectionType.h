#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_DIALOGCHOOSECONNECTIONTYPE_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__

#include "ui_dialogChooseConnectionType.h"

namespace i6engine {
namespace particleEditor {
namespace widgets {

	class MainWindow;

	class DialogChooseConnectionType : public QDialog, public Ui::dialogChooseConnectionType {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		DialogChooseConnectionType(QWidget * par = nullptr);

		/**
		 * \brief destructor
		 */
		~DialogChooseConnectionType();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETPARTICLELIST_H__ */
