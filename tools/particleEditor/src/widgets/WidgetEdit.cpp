#include "widgets/WidgetEdit.h"

#include <QGraphicsView>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetEdit::WidgetEdit(QWidget * par, QWidget * renderWidget) : QWidget(par), _graphicsView(new QGraphicsView(this)) {
		setupUi(this);

		verticalLayout->addWidget(_graphicsView);
		verticalLayout->addWidget(renderWidget);

		verticalLayout->setStretch(0, 1);
		verticalLayout->setStretch(1, 1);
	}

	WidgetEdit::~WidgetEdit() {
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6engine */
