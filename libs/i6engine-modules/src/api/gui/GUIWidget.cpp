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

#include "i6engine/api/gui/GUIWidget.h"

#include <thread>

#include "i6engine/utils/Exceptions.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/CEGUI.h"

namespace i6e {
namespace api {

	GUIWidget::GUIWidget(const std::string & name) : _name(name), _window(), _parent(nullptr), _childs(), _ticking(false), _mouseOverCallback(), _dropable(false), _canDrop(), _dragable(false), _dropCallback(), _originalPos(), _isDragged(false), _dragOffset(), _clickCallback(), _tooltip(), _tooltipActive(false), _animations(), _hitTestBuffer(nullptr), _hitBufferCapacity(), _hitBufferSize(), _hitBufferInverted(false), _transparencyCheckEnabled(false) {
	}

	GUIWidget::~GUIWidget() {
		if (!_parent) {
			_window->destroy();
		}
		delete[] _hitTestBuffer;
	}

	void GUIWidget::update(uint16_t type, gui::GUIUpdateMessageStruct * message) {
		if (type == api::gui::GuiSetPosition) {
			gui::GUI_Position * g = static_cast<gui::GUI_Position *>(message);
			setPosition(g->x, g->y);
			_originalPos.setX(float(g->x));
			_originalPos.setY(float(g->y));
		} else if (type == api::gui::GuiSetSize) {
			gui::GUI_Size * g = static_cast<gui::GUI_Size *>(message);
			setSize(g->width, g->height);
		} else if (type == api::gui::GuiSetVisible) {
			bool vis = static_cast<gui::GUI_Visibility *>(message)->visible;
			_window->setVisible(vis);
			for (GUIWidget * child : _childs) {
				child->update(type, message);
			}
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
				api::EngineController::GetSingleton().getGUIFacade()->setText(_name + "_Tooltip", _tooltip);
			}
		} else if (type == gui::GUIMessageTypes::GuiSetProperty) {
			_window->setProperty(dynamic_cast<gui::GUI_SetProperty *>(message)->prop, dynamic_cast<gui::GUI_SetProperty *>(message)->value);
		} else if (type == gui::GUIMessageTypes::GuiPlayAnimation) {
			CEGUI::AnimationInstance * instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(dynamic_cast<gui::GUI_PlayAnimation *>(message)->animation);
			instance->setTargetWindow(_window);
			instance->start();
			_animations.insert(std::make_pair(dynamic_cast<gui::GUI_PlayAnimation *>(message)->animation, instance));
		} else if (type == gui::GUIMessageTypes::GuiStopAnimation) {
			auto it = _animations.find(dynamic_cast<gui::GUI_StopAnimation *>(message)->animation);
			it->second->stop();
			_animations.erase(it);
		} else if (type == gui::GUIMessageTypes::GuiPauseAnimation) {
			auto it = _animations.find(dynamic_cast<gui::GUI_PauseAnimation *>(message)->animation);
			it->second->pause();
		} else if (type == gui::GUIMessageTypes::GuiUnpauseAnimation) {
			auto it = _animations.find(dynamic_cast<gui::GUI_UnpauseAnimation *>(message)->animation);
			it->second->unpause();
		} else if (type == gui::GUIMessageTypes::GuiSetTransparencyCheck) {
			bool enabled = dynamic_cast<gui::GUI_SetTransparencyCheck *>(message)->enabled;
			if (_transparencyCheckEnabled != enabled) {
				_transparencyCheckEnabled = enabled;
				if (_transparencyCheckEnabled) {
					_window->setUsingAutoRenderingSurface(true);

					CEGUI::RenderingSurface* rs = _window->getRenderingSurface();
					if (rs) {
						rs->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded, CEGUI::Event::Subscriber(&api::GUIWidget::renderingEndedHandler, dynamic_cast<api::GUIWidget *>(this)));
					}
				} else {
					_window->setUsingAutoRenderingSurface(false);

					CEGUI::RenderingSurface* rs = _window->getRenderingSurface();
					if (rs) {
						rs->removeEvent(CEGUI::RenderingSurface::EventRenderQueueEnded);
					}

					delete[] _hitTestBuffer;
					_hitTestBuffer = nullptr;
					_hitBufferCapacity = 0;
				}
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
			i6eGUIFacade->addTicker(this);
		} else {
			i6eGUIFacade->removeTicker(this);
		}
		_ticking = enabled;
	}

	void GUIWidget::setPosition(double x, double y) {
		_window->setPosition(CEGUI::UVector2(CEGUI::UDim(float(x), 0.0f), CEGUI::UDim(float(y), 0.0f)));
	}

	void GUIWidget::setSize(double w, double h) {
		_window->setSize(CEGUI::USize(CEGUI::UDim(float(w), 0.0f), CEGUI::UDim(float(h), 0.0f)));
	}

	bool GUIWidget::isHit() const {
		CEGUI::Vector2f position = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
		if (!_window->isHit(position, false)) {
			return false;
		}

		if (!_hitTestBuffer) {
			return true;
		}

		const CEGUI::Vector2f wpos(CEGUI::CoordConverter::screenToWindow(*_window, position));
		const size_t idx = (_hitBufferInverted ? _hitBufferSize.getY() - wpos.d_y : wpos.d_y) * _hitBufferSize.getX() + wpos.d_x;

		return (_hitTestBuffer[idx] >> 24) > 0;
	}

	bool GUIWidget::renderingEndedHandler(const CEGUI::EventArgs & args) {
		if (!_transparencyCheckEnabled) {
			return false;
		}
		if (static_cast<const CEGUI::RenderQueueEventArgs &>(args).queueID != CEGUI::RQ_BASE) {
			return false;
		}

		// rendering surface needs to exist and needs to be texture backed
		CEGUI::RenderingSurface * const rs = _window->getRenderingSurface();
		if (!rs || !rs->isRenderingWindow()) {
			return false;
		}

		CEGUI::TextureTarget & tt = static_cast<CEGUI::RenderingWindow * const>(rs)->getTextureTarget();

		CEGUI::Texture & texture = tt.getTexture();
		const CEGUI::Sizef tex_sz(texture.getSize());
		const size_t reqd_capacity = size_t(tex_sz.d_width) * size_t(tex_sz.d_height);

		// see if we need to reallocate buffer:
		if (reqd_capacity > _hitBufferCapacity) {
			delete[] _hitTestBuffer;
			_hitTestBuffer = nullptr;
			_hitBufferCapacity = 0;
		}

		// allocate buffer to hold data if it's not already allocated
		if (!_hitTestBuffer) {
			_hitTestBuffer = new uint32_t[reqd_capacity];
			_hitBufferCapacity = reqd_capacity;
		}

		// save details about what will be in the buffer
		_hitBufferInverted = tt.isRenderingInverted();
		_hitBufferSize = Vec2f(tex_sz.d_width, tex_sz.d_height);

		// grab a copy of the data.
		texture.blitToMemory(_hitTestBuffer);

		return true;
	}

	bool GUIWidget::drag(const CEGUI::EventArgs & e) {
		if (!_clickCallback.empty()) {
			std::thread(_clickCallback).detach();
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

	bool GUIWidget::drop(const CEGUI::EventArgs &) {
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

	bool GUIWidget::mouseEnter(const CEGUI::EventArgs &) {
		if (_tooltip.empty() || _tooltipActive) {
			return true;
		}
		_tooltipActive = true;
		api::EngineController::GetSingleton().getGUIFacade()->createWidget(_name + "_Tooltip", "GUITooltip", "");
		api::EngineController::GetSingleton().getGUIFacade()->setSize(_name + "_Tooltip", 0.3, 0.05);
		api::EngineController::GetSingleton().getGUIFacade()->setText(_name + "_Tooltip", _tooltip);
		return true;
	}

	bool GUIWidget::mouseLeave(const CEGUI::EventArgs &) {
		if (_isDragged) {
			setPosition(CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
			enableTicking(true);
		}
		if (_tooltip.empty() || !_tooltipActive) {
			return true;
		}
		_tooltipActive = false;
		api::EngineController::GetSingleton().getGUIFacade()->deleteWidget(_name + "_Tooltip");
		return true;
	}

	void GUIWidget::tick() {
		if (_isDragged) {
			setPosition(CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_x / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_width - _dragOffset.getX(), CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition().d_y / CEGUI::System::getSingleton().getRenderer()->getDisplaySize().d_height - _dragOffset.getY());
		}
	}

} /* namespace api */
} /* namespace i6e */
