#include "i6engine/editor/widgets/RenderWidget.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"
#include <QKeyEvent>

namespace i6engine {
namespace editor {
namespace widgets {

	RenderWidget::RenderWidget(QWidget * par) : QWidget(par) {
		setupUi(this);

		setMouseTracking(true);
		installEventFilter(this);
	}

	RenderWidget::~RenderWidget() {
	}

	void RenderWidget::mouseMoveEvent(QMouseEvent * evt) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(evt->pos().x(), evt->pos().y()), core::Subsystem::Input));
		evt->accept();
	}

	bool RenderWidget::eventFilter(QObject * obj, QEvent * evt) {
		QWidget * srcWidget = qobject_cast<QWidget *>(obj);
		switch (evt->type()) {
		case QEvent::HoverMove:
		case QEvent::NonClientAreaMouseMove:
		case QEvent::MouseMove: {
			QMouseEvent * me = static_cast<QMouseEvent *>(evt);
			mouseMoveEvent(me);
			break;
		}
		}
		return QWidget::eventFilter(obj, evt);
	}

} /* namespace widgets */
} /* namespace editor */
} /* namespace i6engine */
