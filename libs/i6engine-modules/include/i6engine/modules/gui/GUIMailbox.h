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

#ifndef __I6ENGINE_MODULES_GUIMAILBOX_H__
#define __I6ENGINE_MODULES_GUIMAILBOX_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace modules {

	class GraphicsManager;
	class GUIController;
	class GUIManager;

	/**
	 * \class GUIMailbox
	 * \brief Extracts the contents of incoming messages on the "Gui" channel and calls the corresponding methods of the gui subsystem
	 * \details Please consult the messaging documentation found in the "Related Pages" on top of this page for in-depth usage details.
	 * 	The supported methods are declared in the enum method of GUIConfig: \ref i6e::modules::GUIConfig::method
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
} /* namespace i6e */

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
		 	 	 <td>i6e::modules::GUIConfig::GUI_MOUSE_POS</td>
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
		 	 	 <td>i6e::modules::GUIConfig::GUI_KEY_DOWN</td>
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
		 	 	 <td>i6e::modules::GUIConfig::GUI_KEY_UP</td>
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
		 	 	 <td>i6e::modules::GUIConfig::GUI_CHAR</td>
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
		 	 	 <td>i6e::modules::GUIConfig::GUI_TIMEPULSE</td>
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
		 	 	 <td>i6e::modules::GUIConfig::GUI_MOUSE_LEAVES</td>
		 	 	 <td></td>
		 	 </tr>
		 </table>
		*/

