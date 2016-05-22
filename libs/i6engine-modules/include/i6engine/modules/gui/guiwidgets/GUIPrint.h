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
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUIPRINT_H__
#define __I6ENGINE_MODULES_GUIPRINT_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace CEGUI {
	class Colour;
	class Listbox;
	class ListboxTextItem;
} /* namespace CEGUI */

namespace i6e {
namespace api {
namespace gui {
	enum class Alignment;
} /* namespace gui */
} /* namespace api */
namespace modules {

	class GUIPrint : public api::GUIWidget {
	public:
		/**
		 * \brief Constructor for a new GUI Print.
		 * \param name Name of the new Object.
		 * \param type Type of the new Object.
		 */
		GUIPrint(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUIPrint();

		/**
		 * \brief Method called by incoming messages.
		 * \param type Who should receive the Message.
		 * \param message Message struct.
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * message) override;

		/**
		* \brief Method to be called at every Tick.
		*/
		void tick() override;

	private:
		CEGUI::ListboxTextItem *_lbText;
		CEGUI::Listbox * _lb;
		int64_t _lifeTime;
		api::gui::Alignment _alignment;
		std::string _text;
		double _realPosX;
		double _realPosY;
		uint64_t _startTime;

		/**
		 * \brief Sets the text which should be displayed.
		 * \param text Text to be set.
		 */
		void setText(const std::string & text);

		/**
		 * \brief Sets the Colour of this object.
		 * \param col Colour to be set.
		 */
		void setColour(const CEGUI::Colour & col);

		/**
		 * \brief Sets the Font of this object.
		 * \param font_name
		 */
		void setFont(const std::string & font_name);

		/**
		 * \brief Sets the Lifetime of the Text.
		 * \param lifetime Lifetime in milliseconds the text will be printed.
		 */
		void setTimeToDie(const int64_t lifetime);

		/**
		 * \brief forbidden
		 */
		GUIPrint(const GUIPrint &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIPrint & operator=(const GUIPrint &) = delete;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIPRINT_H__ */

/**
 * @}
 */
