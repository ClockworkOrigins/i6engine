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

#ifndef __I6ENGINE_API_GUIFACADE_H__
#define __I6ENGINE_API_GUIFACADE_H__

#include <map>

#include "i6engine/api/KeyCodes.h"

#include "boost/function.hpp"

namespace i6engine {
namespace api {
namespace gui {
	enum class Alignment;
} /* namespace gui */

	class GUIWidget;

	/**
	 * \class GUIFacade
	 * \brief This class manages the GUIWindows and provides some general API methods that aren't related to a special window
	 *
	 * The Application shall only use methods from this class and get them via GetSingleton()
	 * GUIFacede creates Messages of type GUIMessageType and sends them. GUIMailbox will read this Messages and calls wanted Methods from GUIManager
	 * Some Functionality directly calls Methods from the GUIManager
	 * All Methods with an (m) at the end of the brief description send a Message (m)
	 */
	class ISIXE_MODULES_API GUIFacade {
	public:
		GUIFacade();

		~GUIFacade() {
		}

		/**
		 * \brief Creates the GUI. This function has to be called before you can start creating windows ect. (m)
		 * \param strScheme The Scheme filename which should be loaded (.scheme / XML)
		 * \param strFont The default font set (can be empty)
		 * \param strDefaultFont The default font (out of the set, can be empty)
		 * \param strDefaultMouseImageSet The mouse image set
		 * \param strDefaultMouseImageName The name out of the mouseimageset
		 */
		void startGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) const;

		/**
		 * \brief Add the child_window to the RootWindow. (m)
		 * \details The RootWindow is an invisibly full-screen window on top of your application.
		 * Adding windows to it will form your GUI-Interface
		 * Widgets are automatically added to the RootWindow, when they are created.
		 */
		void addToRootWindow(const std::string & child_window) const;

		/**
		 * \brief Sets the MouseVisibility (m)
		 * \param visibility True to display the mouse, False to hide it.
		 */
		void setMouseVisibility(const bool visibility);

		/**
		 * \brief Delete all windows. Should be used when shutting down. (m)
		 */
		void clearAllWindows();

		/**
		 * \brief Subscribes for an event (Mouse-Click). (m)
		 * \param windowname The name of the window to be connected for an event. This can be either a manually created window or the name of a window specified in the .layout file
		 * \param eventType Type of the event. Note that the window type (e.g. PushButton) has to support this type of event. At the moment there's only one event type namely "Clicked"
		 * \param ptrEventMethod Everytime the Event occurs on this window, the Method leading to this pointer will be called
		 */
		void subscribeEvent(const std::string & windowname, const std::string & eventType, const boost::function<void(void)> & ptrEventMethod) const;

		/**
		 * \brief Enable and Disable Events of Windows
		 * \param b true means enabled, false meands disabled
		 */
		void changeEvent(const std::string & windowname, bool b) const;

		/**
		 * \brief Sets visibility of a window (m)
		 * \param windowname name of the window
		 * \param visibilty
		 */
		void setVisibility(const std::string & windowname, const bool visibility) const;

		/**
		 * \brief Wrapper to set both visibility of a window and the cursor
		 * \param windowname name of the window
		 * \param wVisibilty visibility of the window
		 * \param mVisibilty visibility of the cursor
		 */
		void setWindowAndMouseVisibility(const std::string & windowname, const bool wVisibility, const bool mVisibility);

		/**
		 * \brief Adding a Row to a List (m)
		 * \param name the name of the window
		 */
		void addRowToList(const std::string & name) const;

		/**
		 * \brief quite self-explanatory. See addRowToList. (m)
		 */
		void addRowEntry(const std::string & name, const uint32_t row, const uint32_t column, const std::string & entry) const;

		/**
		 * \brief Clears all data in a Widget.
		 *
		 * Removes for example all entries in a StatusList.
		 */
		void clearWidget(const std::string & name) const;

		/**
		 * \brief Adds a new Text to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param message Message to be printed.
		 * \param lifetime Lifetime of the object or -1 for infinite lifetime (default = -1), time in microseconds.
		 */
		void addPrint(const std::string & name, const std::string & type, const double x, const double y, const std::string & message, gui::Alignment alignment, const int64_t lifetime = -1);

		/**
		 * \brief Adds a new ProgressBar to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the Bar
		 * \param h Height of the Bar
		 */
		void addProgressBar(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h);

		/**
		 * \brief Set a text to an specified object.
		 * \param name Name of the object.
		 * \param text Text to be set.
		 */
		void setText(const std::string & name, const std::string & text);

		/**
		 * \brief Sets the Position of an specified object.
		 * \param name Name of the object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 */
		void setPosition(const std::string & name, const double x, const double y);

		/**
		 * \brief Sets the Size of an specified object.
		 * \param name Name of the object.
		 * \param w Width
		 * \param h Height
		 */
		void setSize(const std::string & name, const double w, const double h);

		/**
		 * \brief Sets the amount of items of an specified object.
		 * \param name Name of the object.
		 * \param amount Amount of items
		 */
		void setAmount(const std::string & name, const uint32_t amount);

		/**
		 * \brief Sets the Colour of specified.
		 * \param name Name of the object.
		 * \param alpha Alpha value.
		 * \param red Red value.
		 * \param green Green value.
		 * \param blue Blue value.
		 */
		void setColour(const std::string & name, double alpha, double red, double green, double blue);

		/**
		 * \brief Sets the Font of an specified object.
		 * \param name Name of the object.
		 * \param font Font to be set.
		 */
		void setFont(const std::string & name, const std::string & font);

		/**
		 * \brief Sets the remaining lifetime of a specified object.
		 * \param name Name of the object.
		 * \param time Remaining lifetime of the Object, in microseconds
		 */
		void setLifetime(const std::string & name, const int64_t time);

		/**
		 * \brief Set an aligment to an specified object.
		 * \param name Name of the object.
		 * \param alignment the alignment of the text
		 */
		void setAlignment(const std::string & name, gui::Alignment alignment) const;

		/**
		 * \brief Creates a StatusList-GuiWidget
		 * \param	CEGUI Type i.e. "Tri6/Blanko"
		 * \param	position x-coordinate
		 * \param	position y-coordinate
		 * \param	time before entire List hides, in microseconds
		 * \return   nothing
		 */
		void addStatusList(const std::string & name, const std::string & type, const double x, const double y, const int64_t lifetime);

		/**
		 * \brief Sets the Progress of a bar.
		 * \param name Name of the Widget
		 * \param progress Percentage to set (between 0 and 1)
		 */
		void setProgress(const std::string & name, const double progress);

		/**
		 * \brief Adds a new Image to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param imageSetName imageSet
		 * \param imageName image
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the image
		 * \param h Height of the image
		 */
		void addImage(const std::string & name, const std::string & type, const std::string & imageSetName, const std::string & imageName, const double x, const double y, const double w, const double h);

		/**
		 * \brief Sets the Image
		 * \param name Name of the Widget
		 * \param imageSetName imageSet
		 * \param imageName image
		 */
		void setImage(const std::string & name, const std::string & imageSetName, const std::string & imageSet);

		/**
		 * \brief Adds a text to a widget.
		 * The specific behavior depends on the widget.
		 */
		void addTextToWidget(const std::string & name, const std::string & text) const;

		/**
		 * \brief Registers a new Widget-template.
		 * \param name Name of the Widget-type
		 * \param createFunc A function to create a widget. It gets a string for the name and a string for the style.
		 *  Tip: You can use boost::factory to give it a GUIWidget constructor
		 */
		void registerWidgetTemplate(const std::string & name, const boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> & createFunc);

		/**
		 * \brief Creates a new Widget.
		 * \param name Name of the Widget
		 * \param type Type of the Widget (either registered by registerWidgetTemplate, or one of the default Types)
		 * \param style Style for the Widget
		 */
		void createWidget(const std::string & name, const std::string & type, const std::string & style);

		/**
		 * \brief Deletes a specified object.
		 * \param name Name of the object to be deleted.
		 */
		void deleteWidget(const std::string & name);

		/**
		 * \brief adds a GUIWidget to be ticked
		 */
		void addTicker(GUIWidget * widget);

		/**
		 * \brief removes a GUIWidget from being ticked
		 */
		void removeTicker(GUIWidget * widget);

		/**
		 * \brief registers the callback for addTicker, call only by GUI subsystem
		 */
		void registerAddTickerCallback(const boost::function<void(GUIWidget *)> & func);

		/**
		 * \brief registers the callback for removeTicker, call only by GUI subsystem
		 */
		void registerRemoveTickerCallback(const boost::function<void(GUIWidget *)> & func);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

		/**
		 * \brief sets image for mouse cursor
		 */
		void setMouseCursorImage(const std::string & image);

		/**
		 * \brief adds given imageset so it can be used with own graphics
		 */
		void addImageset(const std::string & imageset);

		/**
		 * \brief called only by GUI subsystem setting state whether input is captured or not
		 */
		void setInputCaptured(bool captured) {
			_captured = captured;
		}

		bool getInputCaptured() const {
			return _captured;
		}

		void setOnWindow(bool onWindow) {
			_onWindow = onWindow;
		}

		bool getOnWindow() const {
			return _onWindow;
		}

	private:
		bool _onWindow;
		bool _captured;

		/**
		 * \brief forbidden
		 */
		GUIFacade(const GUIFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIFacade & operator=(const GUIFacade &) = delete;

		boost::function<void(GUIWidget *)> _addTickerCallback;

		boost::function<void(GUIWidget *)> _removeTickerCallback;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_GUIFACADE_H__ */

/**
 * @}
 */
