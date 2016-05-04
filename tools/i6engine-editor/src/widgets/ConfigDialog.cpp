#include "i6engine/editor/widgets/ConfigDialog.h"

namespace i6e {
namespace editor {
namespace widgets {

	ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent) {
		setupUi(this);

		movementSpeedLabel->setText(QApplication::tr("Movement Speed"));
		rotationSpeedLabel->setText(QApplication::tr("Rotation Speed"));
	}

	ConfigDialog::~ConfigDialog() {
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
