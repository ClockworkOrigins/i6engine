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

#include "i6engine/modules/gui/guiwidgets/GUIImage.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/facades/GUIFacade.h"

#include "CEGUI/Window.h"
#include "CEGUI/WindowManager.h"

namespace i6e {
namespace modules {

	GUIImage::GUIImage(const std::string & name, const std::string & style) : api::GUIWidget(name), _imageSequence(), _fps(0.0), _looping(false), _startTime(0) {
		CEGUI::WindowManager & wmgr = CEGUI::WindowManager::getSingleton();

		// style has to support parameter "image"
		_window = wmgr.createWindow(style, name);
		_window->setUserData(this);
		_window->setProperty("RiseOnClickEnabled", "False");
	}

	void GUIImage::update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) {
		if (type == api::gui::GuiSetImage) {
			api::gui::GUI_Image * g = static_cast<api::gui::GUI_Image *>(data);
			setImage(g->imageSetName, g->imageName);
		} else if (type == api::gui::GuiSetImageSequence) {
			api::gui::GUI_ImageSequence * g = static_cast<api::gui::GUI_ImageSequence *>(data);
			setImageSequence(g->sequence, g->fps, g->looping);
		} else {
			GUIWidget::update(type, data);
		}
	}

	void GUIImage::setImage(const std::string & imageSetName, const std::string & imageName) {
		_window->setProperty("Image", imageSetName + "/" + imageName);
	}

	void GUIImage::setImageSequence(const std::vector<std::pair<std::string, std::string>> & sequence, double fps, bool looping) {
		_imageSequence = sequence;
		_fps = fps;
		_looping = looping;
		_startTime = i6eEngineController->getCurrentTime();
		if (!sequence.empty()) {
			_window->setProperty("Image", sequence.front().first + "/" + sequence.front().second);
			enableTicking(true);
		}
	}

	void GUIImage::tick() {
		api::GUIWidget::tick();
		uint64_t cT = i6eEngineController->getCurrentTime();
		// animate mouse cursor
		if (std::abs(_fps) > DBL_EPSILON && _imageSequence.size() > 1) {
			uint64_t timeDiff = cT - _startTime;
			if (timeDiff > (_imageSequence.size() - 1) * 1000000.0 / _fps) {
				if (_looping) {
					_startTime += uint64_t((_imageSequence.size() - 1) * 1000000.0 / _fps);
				} else {
					_fps = 0.0;
					i6eGUIFacade->setImage(_name, _imageSequence.back().first, _imageSequence.back().second);
				}
			}
			if (std::abs(_fps) > DBL_EPSILON) {
				size_t index = size_t(std::round((timeDiff * _fps) / 1000000.0));
				_window->setProperty("Image", _imageSequence[index].first + "/" + _imageSequence[index].second);
			}
		}
	}

} /* namespace modules */
} /* namespace i6e */
