#include "widgets/ConfigDialog.h"

namespace i6e {
namespace particleEditor {
namespace widgets {

	ConfigDialog::ConfigDialog(QWidget * parent) : QDialog(parent) {
		setupUi(this);

		boxSizeLabel->setText(QApplication::tr("Box Size"));
	}

	ConfigDialog::~ConfigDialog() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
