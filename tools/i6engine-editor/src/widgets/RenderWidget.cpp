#include "i6engine/editor/widgets/RenderWidget.h"

namespace i6engine {
namespace editor {
namespace widgets {

	RenderWidget::RenderWidget(QWidget * par) : QWidget(par) {
		setupUi(this);
	}

	RenderWidget::~RenderWidget() {
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
