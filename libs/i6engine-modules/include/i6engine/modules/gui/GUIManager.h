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

#ifndef __I6ENGINE_MODULES_GUIMANAGER_H__
#define __I6ENGINE_MODULES_GUIMANAGER_H__

#include <array>
#include <list>
#include <map>
#include <sstream>
#include <vector>

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/facades/InputFacade.h"

#include "i6engine/modules/gui/GUIFactory.h"

#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

namespace CEGUI {
	class EventArgs;
	class OgreRenderer;
	class Window;
} /* namespace CEGUI */

namespace Ogre {
	class Root;
} /* namespace Ogre */

namespace i6e {
namespace core {
	struct MessageStruct;
} /* namespace core */
namespace api {
	class GUIWidget;

namespace gui {
	enum class SubscribeEvent;

	struct GUIUpdateMessageStruct;
} /* namespace gui */
} /* namespace api */
namespace modules {

	const std::string GUIMAN = "GUIManager"; // i.e. GUI Manager Name

	enum class KeyState;
	class GUICanvas;
	class GUIController;

	/**
	 * \class GUIManager
	 * \brief This class interacts with CEGUI. It's methods are executed in the OGRE / CEGUI thread
	 */
	class GUIManager {
		friend class GUICanvas;
		friend class GUIController;
		friend class GUIMailbox;

	public:
		typedef std::map<std::string, boost::function<void(void)>> GUIFunctions;

		typedef std::map<std::pair<api::KeyCode, std::string>, boost::function<void(void)>> GUIKeyFunctions;

		/**
		 * \brief constructor, registers basic GUI stuff
		 */
		GUIManager(GUIController * ctrl);

		/**
		 * \brief destructor, cleans up GUI
		 */
		~GUIManager();

		/**
		 * \brief Automatically sets up all necessary dat structures of the underlying gui library.
		 *
		 *     initializes CEGUI by calling its initialization functions
		 */
		void initializeGUI(Ogre::Root * root);

		/**
		 * \brief Returns a pointer to gui root window.
		 * \return Pointer to the gui root window
		 */
		inline CEGUI::Window * getRootWindow() const { return _objRoot; }

		/**
		 * \brief Adds the given function associated with the window to the GUIFunctions map
		 *
		 *     adds a function pointer to _objGUIFunctions by name
		 *     (function is called on clicks)
		 *
		 * \param name Name of the associated window
		 * \param ptrEventMethod Pointer to the designated function
		 */
		void setFunction(const std::string & name, const boost::function<void(void)> & ptrEventMethod);

		/**
		 * \brief returns true, if the event is allowed to be called
		 *
		 *    returns value depending on whether or not _enabledFunctions count is equal to zero
		 *
		 * \param   const std::string & name
		 * \return   bool true if enabled, false otherwise
		 */
		bool allowedEvent(const std::string & name);

		/**
		 * \brief changes the status of the event
		 */
		inline void changeEvent(const std::string & name, const bool b) { _enabledFunctions[name] = b; }

		/**
		 * \brief return visibility of the window
		 *
		 *    returns boolean whether window specified by name is set to visible or not
		 *
		 * \param   const std::string & windowname
		 * \return   bool - true on visible, false otherwise
		 */
		bool getVisibility(const std::string & windowname);

		/**
		 * \brief Handles an incoming message to create a GUI widget
		 * \param type The subtype of the message
		 * \param data The content of the message
		 */
		void processCreateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Handles an incoming message to update a GUI widget
		 * This method forwards the message to the right widget
		 * \param type The subtype of the message
		 * \param data The content of the message
		 */
		void processUpdateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Handles an incoming message to delete a GUI widget
		 * \param type The subtype of the message
		 * \param data The content of the message
		 */
		void processDeleteMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Handles incoming input messages
		 */
		void processInputUpdateMessage(uint16_t type, core::MessageStruct * data);

		/**
		 * \brief registers a widget to be ticked
		 */
		void addTicker(api::GUIWidget * widget);

		/**
		 * \brief removes a widget from the tick list
		 */
		void removeTicker(api::GUIWidget * widget);

		/**
		 * \brief ticks all registered GUIWidgets
		 */
		void tickWidgets();

		/**
		 * \brief registers a widget-template to create new widgets via messages.
		 * \param[in] name Name of the widget-type.
		 * \param[in] createFunc A boost::function which takes a string name and a string style, and returns a GUIWidget *
		 *  Tip: You can use boost::factory to register the Constructor of a GUIWidget.
		 */
		void registerGUIWidgetTemplate(const std::string & name, GUIWidget_factory createFunc);

	protected:
		/**
		 * \brief Creates the GUI.
		 *
		 *     creates new Windows by calling CEGUIs createWindow function
		 *     thereby setting a default mouse cursor
		 * \param[in] strScheme - the Scheme which should be loaded (.scheme / XML)
		 * \param[in] strFont - the default font set
		 * \param[in] strDefaultFont - the default font (out of the set)
		 * \param[in] strDefaultMouseImageSet - the mouse image set
		 * \param[in] strDefaultMouseImageName - the name out of the mouseimageset
		 */
		void createGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName);

		/**
		 * \brief Injects a mouse movement into the gui subsystem.
		 *
		 *     fakes mouse movement and mouse wheel change by using CEGUI's injectMouseMove and injectMouseWheelChange
		 *     possibly handler for OIS
		 *
		 * \param[in] fltDelatX Change of the cursor`s x position
		 * \param[in] fltDeltaY Change of the cursor`s y position
		 * \param[in] fltDeltaZ Change of the mousewheel position
		 */
		void MouseMove(const double fltDeltaX, const double fltDeltaY, const double fltDeltaZ) const;

		/**
		 * \brief Injects a new absolute cursor position into the gui subsystem.
		 *
		 *     fakes mouse movement by using CEGUI's injectMouseMove
		 *
		 * \param[in] fltPosX New absolute x position of the cursor
		 * \param[in] FltPosY New absolute y position of the cursor
		 */
		void MousePos(const double fltPosX, const double fltPosY) const;

		/**
		 * \brief Injects the press of a mouse button into the gui subsytem.
		 *
		 *     MouseDown handler for OIS, receives events and invokes CEGUI's injectMouseButtonDown
		 *
		 *			Injected mouse buttons: left, right, middle, b3, b4
		 * \param[in] enuButton Button which was pressed
		 */
		void MouseDown(const api::KeyCode enuButton) const;

		/**
		 * \brief Injects the release of a mouse button into the gui subsytem.
		 *
		 *     MouseUp handler for OIS, receives events and invokes CEGUI's injectMouseButtonUp
		 *
		 *			Injected mouse buttons: left, right, middle, b3, b4
		 * \param[in] enuButton Button which was released
		 */
		void MouseUp(const api::KeyCode enuButton) const;

		/**
		 * \brief Injects the press of a keyboard key into the gui subsystem.
		 *
		 *     fakes key press by using CEGUI's injectKeyDown and injectChar
		 *     possibly handler for OIS
		 *
		 * \param[in] intapi::KeyCode Keycode of the keyboard key that was pressed
		 */
		void KeyDown(const api::KeyCode intKeyCode, uint32_t text);

		/**
		 * \brief Injects a hold key
		 */
		void KeyHold(const api::KeyCode intKeyCode);

		/**
		 * \brief Injects the release of a keyboard key into the gui subsystem.
		 *
		 *     fakes key release by using CEGUI's injectKeyUp
		 *     possibly handler for OIS
		 *
		 * \param[in] intapi::KeyCode Keycode of the keyboard key that was released
		 */
		void KeyUp(const api::KeyCode intKeyCode);

		/**
		 * \brief Notifies the gui subsystem that the cursor has left the game window.
		 */
		void MouseLeaves() const;

		/**
		 * \brief Calls the CEGUI method addChildWindow (see CEGUI documentation)
		 *
 		 * \param parent_widget Name of the parent widget
		 * \param child_widget Name of the child widget
		 */
		void addChildWindow(const std::string & parent_widget, const std::string & child_widget);

		/**
		 * \brief Calls the CEGUI method addChildWindow with the root window (see CEGUI documentation)
		 *
		 *     adds a given window to root
		 *
		 * \param child The window to add to root
		 */
		void addToRootWindow(CEGUI::Window * child);

		/**
		 * \brief Calls the CEGUI method setVisible on the mouse cursor (see CEGUI documentation)
		 *
		 *     sets mouse visibility by calling CEGUI's setVisible function
		 *
 		 * \param visibility True to show, false to hide.
 		 */
		void setMouseVisibility(const bool visibility);

		/**
		 * \brief Calls clear on _objGUIWindows
		 *
		 *     calls clear on _objGUIWindows
		 */
		void cleanUpAllWindows();

		/**
		 * \brief Calls the CEGUI method subscribeEvent for mouse actions or keystrokes (see CEGUI documentation)
		 *
		 *     Sets specific EventHandler depending on event type specified by name (i.e. ClickedEventHandler, PressedEventHandler, ReleasedEventHandler)
		 *
		 * \param name The name of the window OR the OIS::api::KeyCode of the designated key.
		 * \param type Type of the event, SubscribeEvent
		 */
		void subscribeEvent(const std::string & name, api::gui::SubscribeEvent type);

		/**
		 * \brief Triggers the previously subscribed event by clicking the window, with the name "name". Only clicking so far available
		 */
		void triggerFunction(const std::string & name);

		/**
		 * \brief Triggers the previously subscribed event for a key "Pressed" or "Released" event.
		 */
		void triggerKeyFunction(const uint32_t keyCode, const std::string & type);

		/**
		 * \brief Handles all create messages, which are for the GUIManager
		 */
		void handleCreateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Handles all update messages, which are for the GUIManager
		 */
		void handleUpdateMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Handles all delete messages, which are for the GUIManager
		 */
		void handleDeleteMessage(uint16_t type, api::gui::GUIUpdateMessageStruct * data);

		/**
		 * \brief Returns a pointer to the renderer of the gui subsystem
		 * \return Pointer to the CEGUI renderer for Ogre
		 */
		inline CEGUI::OgreRenderer * getRenderer() const { return _objRenderer; }

		/**
		 * \brief Tick method of the GUI Subsystem
		 */
		void Tick();

	private:
		/**
		 * \brief This method sends a GUI_EVENT message, when a "Clicked" event occurs, i.e. a button in the GUI is clicked.
		 *
		 *     EventHandler for ClickedEvent
		 *
		 *    See GUIFacade.h SubscribeEvent method documentation for details.
		 * \param e The event passed by the SubscribeEvent method
		 * \return   bool, true on success and failure
		 */
		bool ClickedEventHandler(const CEGUI::EventArgs & e);

		/**
		 * \brief This method sends a GUI_EVENT message, when a Key is pressed.
		 *
		 *     EventHandler for PressedEvent
		 *
		 *			Note that currently EVERY pressed key triggers this event, when at least one KeyEvent with type "Pressed" has been subscribed in the SubscribeKeyEvent method.
		 * \return   bool, true on success and failure
		 */
		bool PressedEventHandler(const CEGUI::EventArgs & e);

		/**
		 * \brief This method sends a GUI_EVENT message, when a Key is released.
		 *
		 *     EventHandler for ReleasedEvent
		 *
		 *			Note that currently EVERY released key triggers this event, when at least one KeyEvent with type "Released" has been subscribed in the SubscribeKeyEvent method.
		 * \return   bool, true on success and failure
		 */
		bool ReleasedEventHandler(const CEGUI::EventArgs & e);

		/**
		 * \brief Gets you a pointer to the widget with the name, if it exists. Otherwise error-handling takes place
		 *
		 * \param name The name of the widget
		 * \return The Widget you are seeking for.
		 */
		api::GUIWidget * getWidgetByName(const std::string & name);

		/**
		 * \brief returns a vector with all GUIWidgets currently under the mouse
		 */
		std::vector<api::GUIWidget *> getMouseoverWidgets() const {
			ASSERT_THREAD_SAFETY_FUNCTION
			return _mouseOverWidgets;
		}

		CEGUI::OgreRenderer * _objRenderer;
		GUIFunctions _objGUIFunctions;
		GUIKeyFunctions _objGUIKeyFunctions;
		CEGUI::Window * _objRoot;

		/**
		 * \brief Map of all widgets.
		 */
		std::map<std::string, api::GUIWidget *> _widgets;
		std::map<std::string, bool> _enabledFunctions;
		uint64_t _listIDs;

		/**
		 * \brief List of all widgets, which are ticked
		 */
		std::list<api::GUIWidget *> _toTick;

		std::array<api::KeyState, 238> _keyStates;

		GUIFactory _factory;

		/**
		 * \brief lock for tickerList
		 */
		mutable boost::mutex _tickerLock;

		GUIController * _ctrl;

		std::string _lastOnWindow;

		std::vector<api::GUIWidget *> _mouseOverWidgets;

		/**
		 * \brief forbidden
		 */
		GUIManager(const GUIManager &) = delete;

		/**
		 * \brief forbidden
		 */
		const GUIManager & operator=(const GUIManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIMANAGER_H__ */

/**
 * @}
 */

/**
 * \page messages_gui_event Messaging: GUI Events
 *
 * This page lists all outgoing messages from the GUI subsystem to the Game. These messages are sent using the GUI_EVENT channel.
 * Every message has a variable "EventType" specifying the type of the occured event.
 *
 * \section gui_event_key_down Key pressed
 *
 * This message is sent when a key is pressed. Note that this message is only sent every 300 milli seconds to prevent window flickering.
 *
 * 	<table>
 *		<tr>
 *			<th>Variable</th>
 *		 	<th>Value/Range</th>
 *		 	<th>Description</th>
 *		</tr>
 *		<tr>
 * 			<td>EventType</td>
 * 			<td>i6e::modules::GUIConfig::GUI_EVENT_KEY_DOWN </td>
 * 			<td></td>
 * 		</tr>
 * 		<tr>
 * 			<td>Keycode</td>
 * 			<td>int</td>
 * 			<td>The keycode containing the information which key was pressed</td>
 * 		</tr>
 * 	</table>
 *
 *	\section gui_event_key_up Key released
 *
 *	This message is sent when a key is released. Note that this message is only sent every 300 milli seconds to prevent window flickering.
 *
 * <table>
 *		<tr>
 *			<th>Variable</th>
 *		 	<th>Value/Range</th>
 *		 	<th>Description</th>
 *		</tr>
 *		<tr>
 * 			<td>EventType</td>
 * 			<td>i6e::modules::GUIConfig::GUI_EVENT_KEY_UP </td>
 * 			<td></td>
 * 		</tr>
 * 		<tr>
 * 			<td>Keycode</td>
 * 			<td>int</td>
 * 			<td>The keycode containing the information which key was released</td>
 * 		</tr>
 * 	</table>
 *
 *	\section gui_event_char_inject Char injected
 *
 *	This message is sent when a char is injected. Note that this message is only sent every 300 milli seconds to prevent window flickering.
 *
 *	 <table>
 *		<tr>
 *			<th>Variable</th>
 *		 	<th>Value/Range</th>
 *		 	<th>Description</th>
 *		</tr>
 *		<tr>
 * 			<td>EventType</td>
 * 			<td>i6e::modules::GUIConfig::GUI_EVENT_CHAR_INJECT </td>
 * 			<td></td>
 * 		</tr>
 * 		<tr>
 * 			<td>TextInt</td>
 * 			<td>int</td>
 * 			<td>Specified the char being injected.</td>
 * 		</tr>
 * 	</table>
 *
 *	\section gui_subscribed_event_occured Event occured
 *
 *	This message is sent when someone has subscribed for an event (e.g. a button was clicked) using the GUIFacade::SubscribeEvent method
 *
 *	 <table>
 *		<tr>
 *			<th>Variable</th>
 *		 	<th>Value/Range</th>
 *		 	<th>Description</th>
 *		</tr>
 *		<tr>
 * 			<td>EventType</td>
 * 			<td>i6e::modules::GUIConfig::GUI_SUBSCRIBED_EVENT_OCCURED </td>
 * 			<td></td>
 * 		</tr>
 * 		<tr>
 * 			<td>Name</td>
 * 			<td>std::string</td>
 * 			<td>The name of the window firing the event.</td>
 * 		</tr>
 * 		<tr>
 * 			<td>Type</td>
 * 			<td>"Clicked", ...</td>
 * 			<td>Type of the fired event. (At the moment only "Clicked" available)</td>
 * 		</tr>
 * 	</table>
 */
