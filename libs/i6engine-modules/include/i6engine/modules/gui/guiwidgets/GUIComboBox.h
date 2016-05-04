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

#ifndef __I6ENGINE_MODULES_GUICOMBOBOX_H__
#define __I6ENGINE_MODULES_GUICOMBOBOX_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace i6e {
namespace modules {

	class GUIComboBox : public api::GUIWidget {
	public:
		/**
		 * \brief Constructor for a new GUI ComboBox.
		 * \param name Name of the new Object.
		 * \param type Type of the new Object.
		 */
		GUIComboBox(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUIComboBox() {}

		/**
		 * \brief Method called by incoming messages.
		 * \param type Who should receive the Message.
		 * \param message Message struct.
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * message) override;

	private:
		std::function<void(std::string)> _selectedCallback;

		bool selectionChanged(const CEGUI::EventArgs & evt);
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUICOMBOBOX_H__ */

/**
 * @}
 */
