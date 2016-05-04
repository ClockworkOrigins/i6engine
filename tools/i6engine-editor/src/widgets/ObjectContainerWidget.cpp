#include "i6engine/editor/widgets/ObjectContainerWidget.h"

#include "i6engine/editor/widgets/ObjectInfoWidget.h"
#include "i6engine/editor/widgets/ObjectListWidget.h"

namespace i6e {
namespace editor {
namespace widgets {

	ObjectContainerWidget::ObjectContainerWidget(QWidget * par) : QWidget(par), objectListWidget(new ObjectListWidget(this, par)), objectInfoWidget(new ObjectInfoWidget(this)) {
		setupUi(this);

		gridLayout->addWidget(objectListWidget, 0, 0);
		gridLayout->addWidget(objectInfoWidget, 1, 0);

		connect(objectListWidget, SIGNAL(selectObject(int64_t)), objectInfoWidget, SLOT(doSelectObject(int64_t)));
		connect(objectInfoWidget, SIGNAL(updateObjectList()), objectListWidget, SLOT(doUpdateObjectList()));
	}

	ObjectContainerWidget::~ObjectContainerWidget() {
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6e */
