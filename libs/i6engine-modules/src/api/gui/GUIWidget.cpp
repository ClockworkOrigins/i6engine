/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "i6engine/api/gui/GUIWidget.h"

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/CEGUI.h"

namespace i6engine {
namespace api {

	GUIWidget::GUIWidget(const std::string & name) : _name(name), _window(), _mouseOverCallback(), _dropable(false), _dragable(false), _dropCallback(), _originalPos(), _isDragged(false), _dragOffset(), _clickCallback() {
	}

	GUIWidget::~GUIWidget() {
		_window->destroy();
	}

	void GUIWidget::init() {
		_window->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GUIWidget::drag, this));
		_window->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&GUIWidget::drop, this));
		_window->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&GUIWidget::mouseMove, this));
	}

	void GUIWidget::update(uint16_t type, gui::GUIUpdateMessageStruct * message) {
		if (type == i6engine::api::gui::GuiSetPosition) {
			gui::GUI_Position * g = static_cast<gui::GUI_Position *>(message);
			setPosition(g->x, g->y);
			_originalPos.setX(float(g->x));
			_originalPos.setY(float(g->y));
		} else if (type == i6engine::api::gui::GuiSetSize) {
			gui::GUI_Size * g = static_cast<gui::GUI_Size *>(message);
			setSize(g->width, g->height);
		} else if (type == i6engine::api::gui::GuiSetVisible) {
			bool vis = static_cast<gui::GUI_Visibility *>(message)->visible;
			_window->setVisible(vis);
		} else if (type == gui::GUIMessageTypes::GuiSetDropTarget) {
			_dropable = dynamic_cast<gui::GUI_SetDropTarget *>(message)->dropable;
		} else if (type == gui::GUIMessageTypes::GuiSetDragable) {
			_dragable = dynamic_cast<gui::GUI_SetDragable *>(message)->dragable;
		} else if (type == gui::GUIMessageTypes::GuiSetDropCallback) {
			_dropCallback = dynamic_cast<gui::GUI_SetDropCallback *>(message)->callback;
		} else if (type == gui::GUIMessageTypes::GuiSubscribeEvent) {
			subscribeClickEvent(dynamic_cast<gui::GUI_SubscribeEvent_Update *>(message)->func);
		}
	}

	void GUIWidget::loadWindowLayout(const std::string & name, const std::string & filename) {
		try {
			CEGUI::Window * objTemp = CEGUI::WindowManager::getSingleton().loadLayoutFromFile(filename);
			_window = objTemp;
			objTemp->setName(name);
		} catch(CEGUI::Exception & e) {
			std::string strMsg = e.getMessage().c_str();
			ISIXE_THROW_API("GUI", strMsg);
		}
	}

	void GUIWidget::enableTicking(bool enabled) {
		if (enabled) {
			EngineController::GetSingletonPtr()->getGUIFacade()->addTicker(this);
		} else {
			EngineController::GetSingletonPtr()->getGUIFacade()->removeTicker(this);
		}
	}

	void GUIWidget::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void GUIWidget::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

	bool GUIWidget::drag(const CEGUI::EventArgs & e) {
		if (_clickCallback != nullptr) {
			_clickCallback();
		}
		if (!_dragable) {
			return true;
		}
		_isDragged = true;
		_dragOffset.setX(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _window->getPosition().d_x.d_scale);
		_dragOffset.setY(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _window->getPosition().d_y.d_scale);
		_window->setAlwaysOnTop(true);
		_window->setMousePassThroughEnabled(true);
		return true;
	}

	bool GUIWidget::drop(const CEGUI::EventArgs & e) {
		if (!_isDragged) {
			return true;
		}
		_isDragged = false;
		bool found = false;
		for (auto w : getAllMouseoverWidgets()) {
			if (w->_name != _name && w->canDrop()) {
				setPosition(w->_window->getPosition().d_x.d_scale, w->_window->getPosition().d_y.d_scale);
				_originalPos.setX(w->_window->getPosition().d_x.d_scale);
				_originalPos.setX(w->_window->getPosition().d_y.d_scale);
				if (_dropCallback != nullptr) {
					_dropCallback(_name, w->_name);
				}
				found = true;
				break;
			}
		}
		if (!found) {
			setPosition(_originalPos.getX(), _originalPos.getY());
		}
		_window->setAlwaysOnTop(false);
		_window->setMousePassThroughEnabled(false);
		return true;
	}

	bool GUIWidget::mouseMove(const CEGUI::EventArgs & e) {
		if (_isDragged) {
			setPosition(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
		}
		return true;
	}

} /* namespace api */
} /* namespace i6engine */
