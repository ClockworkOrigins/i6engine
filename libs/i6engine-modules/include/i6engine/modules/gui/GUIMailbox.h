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

#ifndef __I6ENGINE_MODULES_GUIMAILBOX_H__
#define __I6ENGINE_MODULES_GUIMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace modules {

	class GraphicsManager;
	class GUIController;
	class GUIManager;

	/**
	 * \class GUIMailbox
	 * \brief Extracts the contents of incoming messages on the "Gui" channel and calls the corresponding methods of the gui subsystem
	 * \details Please consult the messaging documentation found in the "Related Pages" on top of this page for in-depth usage details.
	 * 	The supported methods are declared in the enum method of GUIConfig: \ref i6engine::modules::GUIConfig::method
	 */
	class GUIMailbox {
		friend class GraphicsManager;
		friend class GUIController;

	private:
		/**
		 * \brief constructor taking GUIManager
		 */
		explicit GUIMailbox(GUIManager * manager);

		/**
		 * \brief destructor
		 */
		~GUIMailbox();

		/**
		 * \brief See Class-Description
		 * \param objMessage Any message on the channel
		 */
		void News(const api::GameMessage::Ptr & msg) const;

		GUIManager * _manager;

		/**
		 * \brief forbidden
		 */
		GUIMailbox(const GUIMailbox &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIMailbox & operator=(const GUIMailbox &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_GUIMAILBOX_H__ */

/**
 * @}
 */

//---------------------------------------------------------------------------------------------------
		// In-depth description of gui subsystem messages

		/**
		 \page messages_gui Messaging: Gui subsystem

		 Overview of all incoming messages supported by the gui subsystem

		 \section audio_types Supported Message types

		Message channels the gui subsystem subscribes to:

		 \li GUI
		 \li Mouse

		\section gui_mousepos Mouse Placement

		Place the cursor at a certain position within the game window.

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_MOUSE_POS</td>
		 	 	 <td></td>
		 	 </tr>
		 	 <tr>
		 	 	 <td>fltPosX</td>
		 	 	 <td>float</td>
		 	 	 <td>New x position of the cursor</td>
		 	 </tr>
			<tr>
		 	 	 <td>fltPosY</td>
		 	 	 <td>float</td>
		 	 	 <td>New y position of the cursor</td>
		 	 </tr>
		 </table>

		\section gui_keydown Keyboard key pressed

		Inject press of a keyboard key.

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_KEY_DOWN</td>
		 	 	 <td></td>
		 	 </tr>
		 	 <tr>
		 	 	 <td>intKeycode</td>
		 	 	 <td>int</td>
		 	 	 <td>Keycode of the pressed key</td>
		 	 </tr>
		 </table>

		\section gui_keyup Keyboard key released

		Inject release of a keyboard key.

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_KEY_UP</td>
		 	 	 <td></td>
		 	 </tr>
		 	 <tr>
		 	 	 <td>intKeycode</td>
		 	 	 <td>int</td>
		 	 	 <td>Keycode of the released key</td>
		 	 </tr>
		 </table>

		\section gui_char Input of a character

		Inject input of a character (MUST be done to fill textboxes where the injection of the keypress alone is not sufficient for CEGUI).

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_CHAR</td>
		 	 	 <td></td>
		 	 </tr>
		 	 <tr>
		 	 	 <td>intChar</td>
		 	 	 <td>int</td>
		 	 	 <td>Character to be injected.</td>
		 	 </tr>
		 </table>

		\section gui_timepulse Timepulse

		Inject a timepulse to the gui. Necessary for scripted actions and fades.

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_TIMEPULSE</td>
		 	 	 <td></td>
		 	 </tr>
		 	 <tr>
		 	 	 <td>fltDeltaT</td>
		 	 	 <td>float</td>
		 	 	 <td>Time elapsed since the last tick</td>
		 	 </tr>
		 </table>

		\section gui_mouseleaves Cursor leaves the game window

		Notify the gui subsytem, that the cursor has left the game window. Some gui widgets rely on this information to deactivate some of their functions.

		 <table>
		 	 <tr>
		 	 	 <th>Variable</th>
		 	 	 <th>Value/Range</th>
		 	 	 <th>Description</th>
		 	 </tr>
		 	 <tr>
		 	 	 <td>Method</td>
		 	 	 <td>i6engine::modules::GUIConfig::GUI_MOUSE_LEAVES</td>
		 	 	 <td></td>
		 	 </tr>
		 </table>
		*/

