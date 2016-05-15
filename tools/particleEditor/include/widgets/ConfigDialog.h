#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_CONFIGDIALOG_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_CONFIGDIALOG_H__

#include "ui_dialogConfig.h"

#include <QDialog>

namespace i6e {
namespace particleEditor {
namespace widgets {

	class ConfigDialog : public QDialog, public Ui::ConfigDialog {
		Q_OBJECT

	public:
		explicit ConfigDialog(QWidget * parent = nullptr);
		~ConfigDialog();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_CONFIGDIALOG_H__ */
