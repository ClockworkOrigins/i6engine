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

#ifndef __I6ENGINE_MODULES_GUITEXTBUTTON_H__
#define __I6ENGINE_MODULES_GUITEXTBUTTON_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace i6engine {
namespace modules {

	class GUITextButton : public api::GUIWidget {
	public:
		/**
		 * \brief Constructor for a new GUI Print.
		 * \param name Name of the new Object.
		 * \param type Type of the new Object.
		 */
		GUITextButton(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUITextButton();

		/**
		 * \brief Method called by incoming messages.
		 * \param type Who should receive the Message.
		 * \param message Message struct.
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * message) override;

	private:
		/**
		 * \brief forbidden
		 */
		GUITextButton(const GUITextButton &) = delete;

		/**
		 * \brief forbidden
		 */
		GUITextButton & operator=(const GUITextButton &) = delete;
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUITEXTBUTTON_H__ */

/**
 * @}
 */

