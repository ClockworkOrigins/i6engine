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

/**
 * \addtogroup RPG
 * @{
 */

#ifndef __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__
#define __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__

#include <vector>

#include "i6engine/api/gui/GUIWidget.h"

namespace i6e {
namespace rpg {
namespace gui {

	/**
	 * \brief allows subtitles with automatic line break and centering in subtitle widget
	 */
	class ISIXE_RPG_API SubtitleWidget : public api::GUIWidget {
	public:
		/**
		 * \brief type has to be the style for the image, e.g. RPG/StaticImage
		 */
		SubtitleWidget(const std::string & name, const std::string & type);

		~SubtitleWidget();

		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * data) override;

	private:
		std::string _text;
		std::string _font;
		std::vector<CEGUI::Window *> _lines;
		std::string _listboxStyle;

		void setText(const std::string & text);
	};

} /* namespace gui */
} /* namespace rpg */
} /* namespace i6e */

#endif /* __I6ENGINE_RPG_GUI_SUBTITLEWIDGET_H__ */

/**
 * @}
 */
