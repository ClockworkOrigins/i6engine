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
