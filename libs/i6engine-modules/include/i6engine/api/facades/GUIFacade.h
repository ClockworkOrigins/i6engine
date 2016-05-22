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

#ifndef __I6ENGINE_API_GUIFACADE_H__
#define __I6ENGINE_API_GUIFACADE_H__

#include <map>

#include "i6engine/api/KeyCodes.h"

#include "boost/function.hpp"

namespace i6e {
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
	 */
	class ISIXE_MODULES_API GUIFacade {
	public:
		GUIFacade();

		~GUIFacade() {
		}

		/**
		 * \brief Creates the GUI. This function has to be called before you can start creating windows ect.
		 * \param strScheme The Scheme filename which should be loaded (.scheme / XML)
		 * \param strFont The default font set (can be empty)
		 * \param strDefaultFont The default font (out of the set, can be empty)
		 * \param strDefaultMouseImageSet The mouse image set
		 * \param strDefaultMouseImageName The name out of the mouseimageset
		 */
		void startGUI(const std::string & strScheme, const std::string & strFont, const std::string & strDefaultFont, const std::string & strDefaultMouseImageSet, const std::string & strDefaultMouseImageName) const;

		/**
		 * \brief Add the child_window to the RootWindow.
		 * \details The RootWindow is an invisibly full-screen window on top of your application.
		 * Adding windows to it will form your GUI-Interface
		 * Widgets are automatically added to the RootWindow, when they are created.
		 */
		void addToRootWindow(const std::string & child_window) const;

		/**
		 * \brief Sets the MouseVisibility
		 * \param visibility True to display the mouse, False to hide it.
		 */
		void setMouseVisibility(const bool visibility) const;

		/**
		 * \brief Delete all windows. Should be used when shutting down.
		 */
		void clearAllWindows() const;

		/**
		 * \brief Subscribes for an event (Mouse-Click).
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
		 * \brief sets image for mouse cursor
		 */
		void setMouseCursorImage(const std::string & image) const;

		/**
		 * \brief adds given imageset so it can be used with own graphics
		 */
		void addImageset(const std::string & imageset) const;

		/**
		 * \brief loads a canvas file
		 */
		void loadCanvas(const std::string & name, const std::string & file) const;

		/**
		 * \brief Adds a new Text to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param message Message to be printed.
		 * \param lifetime Lifetime of the object or -1 for infinite lifetime (default = -1), time in microseconds.
		 */
		void addPrint(const std::string & name, const std::string & type, const double x, const double y, const std::string & message, gui::Alignment alignment, const int64_t lifetime = -1) const;

		/**
		 * \brief Adds a new ProgressBar to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the Bar
		 * \param h Height of the Bar
		 */
		void addProgressBar(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) const;

		/**
		 * \brief Adds a new Editbox to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the Editbox
		 * \param h Height of the Editbox
		 * \param text default text of the Editbox
		 * \param enterCallback callback to be called when text changed
		 */
		void addEditbox(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & defaultText, const std::function<void(std::string)> & enterCallback) const;

		/**
		 * \brief Adds a new TextButton to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the TextButton
		 * \param h Height of the TextButton
		 * \param text text of the TextButton
		 */
		void addTextButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & text) const;

		/**
		 * \brief Adds a new ImageButton to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the TextButton
		 * \param h Height of the TextButton
		 * \param normalImage normal image of the button
		 * \param hoverImage image of the button when hovered
		 * \param pushedImage image of the button when pushed
		 */
		void addImageButton(const std::string & name, const std::string & type, double x, double y, double w, double h, const std::string & normalImage, const std::string & hoverImage, const std::string & pushedImage) const;

		/**
		 * \brief Adds a new ToggleButton to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param type Style of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the Editbox
		 * \param h Height of the Editbox
		 * \param selected default selection of the ToggleButton
		 * \param selectedCallback callback to be called when selection changed
		 */
		void addToggleButton(const std::string & name, const std::string & type, double x, double y, double w, double h, bool selected, const std::function<void(bool)> & selectedCallback) const;

		/**
		 * \brief Creates a StatusList-GuiWidget
		 * \param	CEGUI Type i.e. "Tri6/Blanko"
		 * \param	position x-coordinate
		 * \param	position y-coordinate
		 * \param	time before entire List hides, in microseconds
		 * \return   nothing
		 */
		void addStatusList(const std::string & name, const std::string & type, const double x, const double y, const int64_t lifetime) const;

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
		void addImage(const std::string & name, const std::string & type, const std::string & imageSetName, const std::string & imageName, const double x, const double y, const double w, const double h) const;

		/**
		 * \brief Adds a MultiColumnList to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the image
		 * \param h Height of the image
		 */
		void addMultiColumnList(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h) const;

		/**
		 * \brief Adds a ComboBox to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the image
		 * \param h Height of the image
		 * \param selectedCallback called when selection in ComboBox changes
		 */
		void addComboBox(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::function<void(std::string)> & selectedCallback) const;

		/**
		 * \brief Adds a Chat to the screen.
		 * \param name (Unique) Name of the new object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 * \param w Width of the image
		 * \param h Height of the image
		 * \param acceptedCallback called when entered text is accepted
		 */
		void addChat(const std::string & name, const std::string & type, const double x, const double y, const double w, const double h, const std::function<std::string(std::string)> & acceptedCallback) const;

		/**
		 * \brief Set a text to an specified object.
		 * \param name Name of the object.
		 * \param text Text to be set.
		 */
		void setText(const std::string & name, const std::string & text) const;

		/**
		 * \brief Sets the Position of an specified object.
		 * \param name Name of the object.
		 * \param x X-Value of the position.
		 * \param y Y-Value of the position.
		 */
		void setPosition(const std::string & name, const double x, const double y) const;

		/**
		 * \brief Sets the Size of an specified object.
		 * \param name Name of the object.
		 * \param w Width
		 * \param h Height
		 */
		void setSize(const std::string & name, const double w, const double h) const;

		/**
		 * \brief Sets visibility of a window
		 * \param windowname name of the window
		 * \param visibilty
		 */
		void setVisibility(const std::string & windowname, const bool visibility) const;

		/**
		 * \brief Sets the amount of items of an specified object.
		 * \param name Name of the object.
		 * \param amount Amount of items
		 */
		void setAmount(const std::string & name, const uint32_t amount) const;

		/**
		 * \brief Sets the Colour of specified.
		 * \param name Name of the object.
		 * \param alpha Alpha value.
		 * \param red Red value.
		 * \param green Green value.
		 * \param blue Blue value.
		 */
		void setColour(const std::string & name, double alpha, double red, double green, double blue) const;

		/**
		 * \brief Sets the Font of an specified object.
		 * \param name Name of the object.
		 * \param font Font to be set.
		 */
		void setFont(const std::string & name, const std::string & font) const;

		/**
		 * \brief Sets the remaining lifetime of a specified object.
		 * \param name Name of the object.
		 * \param time Remaining lifetime of the Object, in microseconds
		 */
		void setLifetime(const std::string & name, const int64_t time) const;

		/**
		 * \brief Set an aligment to an specified object.
		 * \param name Name of the object.
		 * \param alignment the alignment of the text
		 */
		void setAlignment(const std::string & name, gui::Alignment alignment) const;

		/**
		 * \brief Enables or disables Widget as drop target
		 * \param name Name of the object.
		 * \param target true means Widget accepts drops
		 */
		void enableDropTarget(const std::string & name, bool target, const std::function<bool(std::string)> & func) const;

		/**
		 * \brief Enables or disables Widget being dragable
		 * \param name Name of the object.
		 * \param enabled defines whether the Widget is dragable or not
		 */
		void setDragable(const std::string & name, bool enabled) const;

		/**
		 * \brief Sets callback being called when the dragable widget is dropped
		 * \param name Name of the object.
		 * \param callback the method to be called on drop
		 */
		void setDropCallback(const std::string & name, const std::function<void(const std::string &, const std::string &)> & callback) const;

		/**
		 * \brief Sets the Progress of a bar.
		 * \param name Name of the Widget
		 * \param progress Percentage to set (between 0 and 1)
		 */
		void setProgress(const std::string & name, const double progress) const;

		/**
		 * \brief Adding a Row to a List
		 * \param name the name of the window
		 */
		void addRowToList(const std::string & name) const;

		/**
		 * \brief quite self-explanatory. See addRowToList.
		 */
		void addRowEntry(const std::string & name, const uint32_t row, const uint32_t column, const std::string & entry) const;

		/**
		 * \brief Adding a column to a List
		 * \param name the name of the window
		 * \param columnHeader header text of the column
		 * \param column index of the column
		 * \param width width of this column
		 */
		void addColumn(const std::string & name, const std::string & columnHeader, uint32_t column, double width) const;

		/**
		 * \brief Clears all data in a Widget.
		 *
		 * Removes for example all entries in a StatusList.
		 */
		void clearWidget(const std::string & name) const;

		/**
		 * \brief Sets the Image
		 * \param name Name of the Widget
		 * \param imageSetName imageSet
		 * \param imageName image
		 */
		void setImage(const std::string & name, const std::string & imageSetName, const std::string & imageSet) const;

		/**
		 * \brief Sets a enter callback to an Editbox
		 * \param name Name of the Widget
		 * \param enterCallback callback to be called when text changes
		 */
		void setEnterTextCallback(const std::string & name, const std::function<void(std::string)> & enterCallback) const;

		/**
		 * \brief Sets a widgets property
		 * \param name Name of the Widget
		 * \param prop property to be set
		 * \param value new value of the property
		 */
		void setProperty(const std::string & name, const std::string & prop, const std::string & value) const;

		/**
		 * \brief Sets a selection state of widget
		 * \param name Name of the Widget
		 * \param selected true if widget is selected
		 */
		void setSelected(const std::string & name, bool selected) const;

		/**
		 * \brief Sets a selected callback to a ToggleButton
		 * \param name Name of the Widget
		 * \param selectedCallback callback to be called when selection changes
		 */
		void setSelectedCallback(const std::string & name, const std::function<void(bool)> & selectedCallback) const;

		/**
		 * \brief Sets a accepted callback to a chat
		 * \param name Name of the Widget
		 * \param acceptedCallback callback to be called when entered text is accepted
		 */
		void setAcceptedTextCallback(const std::string & name, const std::function<std::string(std::string)> & acceptedCallback) const;

		/**
		 * \brief sets tooltip for a widget
		 * \param name Name of the Widget
		 * \param tooltip the tooltip text... for multilines use ~ as delimiter
		 */
		void setTooltip(const std::string & name, const std::string & tooltip) const;

		/**
		 * \brief Adds a text to a widget.
		 * The specific behavior depends on the widget.
		 */
		void addTextToWidget(const std::string & name, const std::string & text) const;

		/**
		 * \brief Sets a selected callback to a ComboBox/Listbox
		 * \param name Name of the Widget
		 * \param selectedCallback callback to be called when selection changes
		 */
		void setSelectedStringCallback(const std::string & name, const std::function<void(std::string)> & selectedCallback) const;

		/**
		 * \brief plays given animation
		 * \param name Name of the Widget
		 * \param animation the animation to be played
		 */
		void playAnimation(const std::string & name, const std::string & animation) const;

		/**
		 * \brief stops given animation, is deleted
		 * \param name Name of the Widget
		 * \param animation the animation to be played
		 */
		void stopAnimation(const std::string & name, const std::string & animation) const;

		/**
		 * \brief pauses given animation, to run again use unpauseAnimation
		 * \param name Name of the Widget
		 * \param animation the animation to be played
		 */
		void pauseAnimation(const std::string & name, const std::string & animation) const;

		/**
		* \brief unpauses an animation paused before using unpauseAnimation
		* \param name Name of the Widget
		* \param animation the animation to be played
		*/
		void unpauseAnimation(const std::string & name, const std::string & animation) const;

		/**
		 * \brief sets automatic line break to the widget (currently only implemtend for Statuslist)
		 */
		void setAutoLineBreak(const std::string & name, bool enabled) const;

		/**
		 * \brief Registers a new Widget-template.
		 * \param name Name of the Widget-type
		 * \param createFunc A function to create a widget. It gets a string for the name and a string for the style.
		 *  Tip: You can use boost::factory to give it a GUIWidget constructor
		 */
		void registerWidgetTemplate(const std::string & name, const boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> & createFunc) const;

		/**
		 * \brief Creates a new Widget.
		 * \param name Name of the Widget
		 * \param type Type of the Widget (either registered by registerWidgetTemplate, or one of the default Types)
		 * \param style Style for the Widget
		 */
		void createWidget(const std::string & name, const std::string & type, const std::string & style) const;

		/**
		 * \brief Deletes a specified object.
		 * \param name Name of the object to be deleted.
		 */
		void deleteWidget(const std::string & name) const;

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
		 * \brief called only by GUI subsystem setting state whether input is captured or not
		 * call this only from within GUIManager
		 */
		void setInputCaptured(bool captured) {
			_captured = captured;
		}

		/**
		 * \brief returns true, if the input is captured in a GUI widget
		 */
		bool getInputCaptured() const {
			return _captured;
		}

		/**
		 * \brief sets whether mouse in on a GUI window or not
		 * call this only from within GUIManager
		 */
		void setOnWindow(bool onWindow) {
			_onWindow = onWindow;
		}

		/**
		 * \brief returns true if mouse is above a visible GUI widget element
		 * transparent GUI parts aren't recognized
		 */
		bool getOnWindow() const {
			return _onWindow;
		}

		/**
		 * \brief sets default font to be used
		 */
		void setDefaultFont(const std::string & font) const;

		/**
		 * \brief adds a widget as child of another one
		 */
		void addWidgetAsChildTo(const std::string & child, const std::string & parent) const;

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_GUIFACADE_H__ */

/**
 * @}
 */
