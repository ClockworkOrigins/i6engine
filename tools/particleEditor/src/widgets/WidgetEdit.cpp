#include "widgets/WidgetEdit.h"

#include "widgets/WidgetEditComponent.h"

#include <QGraphicsScene>
#include <QGraphicsView>

namespace i6engine {
namespace particleEditor {
namespace widgets {

	WidgetEdit::WidgetEdit(QWidget * par, QWidget * renderWidget) : QWidget(par), _graphicsScene(new QGraphicsScene(this)), _graphicsView(new QGraphicsView(_graphicsScene)) {
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
