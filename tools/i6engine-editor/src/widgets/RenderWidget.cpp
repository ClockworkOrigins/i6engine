/*
 * i6engine
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of i6engine; i6engine is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "i6engine/editor/widgets/RenderWidget.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/GameMessage.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include <QMouseEvent>

namespace i6e {
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
		i6eMessagingFacade->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::mouse::MouMouse, core::Method::Update, new api::input::Input_Mouse_Update(evt->pos().x(), evt->pos().y()), core::Subsystem::Input));
		evt->accept();
	}

	bool RenderWidget::eventFilter(QObject * obj, QEvent * evt) {
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
} /* namespace i6e */
