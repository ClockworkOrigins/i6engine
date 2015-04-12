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

	GUIWidget::GUIWidget(const std::string & name) : _name(name), _window(), _ticking(false), _mouseOverCallback(), _dropable(false), _canDrop(), _dragable(false), _dropCallback(), _originalPos(), _isDragged(false), _dragOffset(), _clickCallback(), _tooltip(), _tooltipActive(false) {
	}

	GUIWidget::~GUIWidget() {
		_window->destroy();
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
			_canDrop = dynamic_cast<gui::GUI_SetDropTarget *>(message)->func;
		} else if (type == gui::GUIMessageTypes::GuiSetDragable) {
			_dragable = dynamic_cast<gui::GUI_SetDragable *>(message)->dragable;
			_window->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GUIWidget::drag, this));
			_window->subscribeEvent(CEGUI::Window::EventMouseButtonUp, CEGUI::Event::Subscriber(&GUIWidget::drop, this));
			_window->subscribeEvent(CEGUI::Window::EventMouseMove, CEGUI::Event::Subscriber(&GUIWidget::mouseMove, this));
			_window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GUIWidget::mouseLeave, this));
		} else if (type == gui::GUIMessageTypes::GuiSetDropCallback) {
			_dropCallback = dynamic_cast<gui::GUI_SetDropCallback *>(message)->callback;
		} else if (type == gui::GUIMessageTypes::GuiSubscribeEvent) {
			subscribeClickEvent(dynamic_cast<gui::GUI_SubscribeEvent_Update *>(message)->func);
			_window->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&GUIWidget::drag, this));
		} else if (type == gui::GUIMessageTypes::GuiSetTooltip) {
			_tooltip = dynamic_cast<gui::GUI_SetTooltip *>(message)->tooltip;
			_window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, CEGUI::Event::Subscriber(&GUIWidget::mouseEnter, this));
			_window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, CEGUI::Event::Subscriber(&GUIWidget::mouseLeave, this));
			if (_tooltipActive) {
				i6engine::api::EngineController::GetSingleton().getGUIFacade()->setText(_name + "_Tooltip", _tooltip);
			}
		} else {
			ISIXE_THROW_API("GUI", "Don't know what to do with " << type);
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
		_ticking = enabled;
	}

	void GUIWidget::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void GUIWidget::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

	bool GUIWidget::drag(const CEGUI::EventArgs & e) {
		if (!_clickCallback.empty()) {
			_clickCallback();
		}
		if (!_dragable) {
			return false;
		}
		_isDragged = true;
		_dragOffset.setX(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _window->getPosition().d_x.d_scale);
		_dragOffset.setY(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _window->getPosition().d_y.d_scale);
		_window->setAlwaysOnTop(true);
		return true;
	}

	bool GUIWidget::drop(const CEGUI::EventArgs & e) {
		if (!_isDragged) {
			return true;
		}
		_isDragged = false;
		bool found = false;
		for (auto w : getAllMouseoverWidgets()) {
			if (w->_name != _name && w->canDrop() && w->_canDrop(_name)) {
				setPosition(w->_window->getPosition().d_x.d_scale, w->_window->getPosition().d_y.d_scale);
				_originalPos.setX(w->_window->getPosition().d_x.d_scale);
				_originalPos.setY(w->_window->getPosition().d_y.d_scale);
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
		return true;
	}

	bool GUIWidget::mouseMove(const CEGUI::EventArgs & e) {
		if (_isDragged) {
			setPosition(dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), dynamic_cast<const CEGUI::MouseEventArgs *>(&e)->position.d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
			if (_ticking) {
				enableTicking(false);
			}
		}
		return true;
	}

	bool GUIWidget::mouseEnter(const CEGUI::EventArgs & e) {
		if (_tooltip.empty() || _tooltipActive) {
			return true;
		}
		_tooltipActive = true;
		i6engine::api::EngineController::GetSingleton().getGUIFacade()->createWidget(_name + "_Tooltip", "GUITooltip", "");
		i6engine::api::EngineController::GetSingleton().getGUIFacade()->setSize(_name + "_Tooltip", 0.3, 0.05);
		i6engine::api::EngineController::GetSingleton().getGUIFacade()->setText(_name + "_Tooltip", _tooltip);
		return true;
	}

	bool GUIWidget::mouseLeave(const CEGUI::EventArgs & e) {
		if (_isDragged) {
			setPosition(CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
			enableTicking(true);
		}
		if (_tooltip.empty() || !_tooltipActive) {
			return true;
		}
		_tooltipActive = false;
		i6engine::api::EngineController::GetSingleton().getGUIFacade()->deleteWidget(_name + "_Tooltip");
		return true;
	}

	void GUIWidget::tick() {
		if (_isDragged) {
			setPosition(CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
		}
	}

} /* namespace api */
} /* namespace i6engine */
