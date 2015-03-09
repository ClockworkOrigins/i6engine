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

#ifndef __I6ENGINE_API_GUI_GUICONFIG_H__
#define __I6ENGINE_API_GUI_GUICONFIG_H__

#include "i6engine/api/GameMessageStruct.h"
#include "i6engine/api/KeyCodes.h"
#include "i6engine/api/configs/GraphicsConfig.h"

#include "boost/function.hpp"

namespace i6engine {
namespace api {
	class GUIWidget;
namespace gui {
	enum GUIMessageTypes {
		GuiReset = 0,
		GuiAdd,
		GuiXml,
		GuiMousePos,
		GuiKeyDown,
		GuiKeyUp,
		GuiGuiChar,
		GuiTimepulse,
		GuiMouseLeave,
		GuiChild,
		GuiSubscribeEvent,
		GuiEvent,
		GuiWidgetTemplate,
		GuiWindow,
		GuiAddToRoot,
		GuiMouseVisible,
		GuiFillRow,
		GuiCleanUp,
		GuiAddWidget,
		GuiDeleteWidget,
		GuiSetText,
		GuiAddText,
		GuiSetColour,
		GuiSetPosition,
		GuiSetSize,
		GuiSetLifetime,
		GuiSetFont,
		GuiSetVisible,
		GuiSetProgress,
		GuiAddRow,
		GuiSetRowEntry,
		GuiClearWidget,
		GuiSetAmount,
		GuiSetImage,
		GuiMouseCursorImage,
		GuiResolution,
		GuiAddImageset,
		GuiSetAlignment,
		GuiSetDropTarget,
		GuiSetDragable,
		GuiSetDropCallback,
		GuiMessageTypesCount
	};

	enum class Alignment {
		Left,
		Center,
		Right
	};

	/**
	 * \class GUIUpdateMessageStruct
	 * \brief Used to partially unwrap GUI messages
	 */
	typedef struct ISIXE_MODULES_API GUIUpdateMessageStruct : GameMessageStruct {
		std::string _name;

		GUIUpdateMessageStruct() {}
		GUIUpdateMessageStruct(const std::string & name) : GameMessageStruct(), _name(name) {}
		/*GUIUpdateMessageStruct(const std::string & name, int64_t i, const core::IPKey & r, const core::IPKey & s, int64_t waitID) : GameMessageStruct(i, r, s, waitID), _name(name) {
		}*/

		~GUIUpdateMessageStruct() {}

		GUIUpdateMessageStruct * copy() {
			return new GUIUpdateMessageStruct(*this);
		}
	} GUIUpdateMessageStruct;

	/**
	 * \brief creates given Widget
	 */
	typedef struct GUI_Widget_Create : GUIUpdateMessageStruct {
		std::string widgetType;
		std::string style;

		GUI_Widget_Create() {}
		GUI_Widget_Create(const std::string & name, const std::string & widget_type, const std::string & sty) : GUIUpdateMessageStruct(name), widgetType(widget_type), style(sty) {
		}
		GUI_Widget_Create * copy() {
			return new GUI_Widget_Create(*this);
		}
	} GUI_Widget_Create;

	/**
	 * \brief sets text to Widget
	 */
	typedef struct GUI_Text : GUIUpdateMessageStruct {
		std::string text;
		GUI_Text() {}
		GUI_Text(const std::string & name, const std::string & txt) : GUIUpdateMessageStruct(name), text(txt) {
		}
		GUI_Text * copy() {
			return new GUI_Text(*this);
		}
	} GUI_Text;

	/**
	 * \brief sets image to Widget
	 */
	typedef struct GUI_Image : GUIUpdateMessageStruct {
		std::string imageSetName;
		std::string imageName;
		GUI_Image() {}
		GUI_Image(const std::string & name, const std::string & imageSetName2, const std::string & imageName2) : GUIUpdateMessageStruct(name), imageSetName(imageSetName2), imageName(imageName2) {
		}
		GUI_Image * copy() {
			return new GUI_Image(*this);
		}
	} GUI_Image;

	/**
	 * \brief sets color for Widget
	 */
	typedef struct GUI_Colour : GUIUpdateMessageStruct {
		double alpha, red, green, blue;
		GUI_Colour() {}
		GUI_Colour(const std::string & name, const double a, const double r, const double g, const double b) : GUIUpdateMessageStruct(name), alpha(a), red(r), green(g), blue(b) {
		}
		GUI_Colour * copy() {
			return new GUI_Colour(*this);
		}
	} GUI_Colour;

	/**
	 * \brief sets position for Widget
	 */
	typedef struct GUI_Position : GUIUpdateMessageStruct {
		double x, y;
		GUI_Position() {}
		GUI_Position(const std::string & name, const double xPos, const double yPos) : GUIUpdateMessageStruct(name), x(xPos), y(yPos) {
		}
		GUI_Position * copy() {
			return new GUI_Position(*this);
		}
	} GUI_Position;

	/**
	 * \brief sets lifetime for Widget (Prints)
	 */
	typedef struct GUI_Lifetime : GUIUpdateMessageStruct {
		int64_t lifetime;
		GUI_Lifetime() {}
		GUI_Lifetime(const std::string & name, const int64_t time) : GUIUpdateMessageStruct(name), lifetime(time) {
		}
		GUI_Lifetime * copy() {
			return new GUI_Lifetime(*this);
		}
	} GUI_Lifetime;

	/**
	 * \brief sets visibility of the Widget
	 */
	typedef struct GUI_Visibility : GUIUpdateMessageStruct {
		bool visible;
		GUI_Visibility() {}
		GUI_Visibility(const std::string & name, const bool vis) : GUIUpdateMessageStruct(name), visible(vis) {
		}
		GUI_Visibility * copy() {
			return new GUI_Visibility(*this);
		}
	} GUI_Visibility;

	/**
	 * \brief sets progress of the Widget (for ProgressBar)
	 */
	typedef struct GUI_Progress : GUIUpdateMessageStruct {
		double progress;
		GUI_Progress() {}
		GUI_Progress(const std::string & name, const double prog) : GUIUpdateMessageStruct(name), progress(prog) {
		}
		GUI_Progress * copy() {
			return new GUI_Progress(*this);
		}
	} GUI_Progress;

	/**
	 * \brief sets amount of rows for MultiColumnList
	 */
	typedef struct GUI_Amount : GUIUpdateMessageStruct {
		uint32_t amount;
		GUI_Amount() {}
		GUI_Amount(const std::string & name, const uint32_t a) : GUIUpdateMessageStruct(name), amount(a) {
		}
		GUI_Amount * copy() {
			return new GUI_Amount(*this);
		}
	} GUI_Amount;

	/**
	 * \brief adds an entry to a MultiColumnList
	 */
	typedef struct GUI_Add_Row_Entry : GUIUpdateMessageStruct {
		uint32_t row;
		uint32_t column;
		std::string entry;
		GUI_Add_Row_Entry() {}
		GUI_Add_Row_Entry(const std::string & name, const uint32_t r, const uint32_t c, const std::string & e) : GUIUpdateMessageStruct(name), row(r), column(c), entry(e) {
		}
		GUI_Add_Row_Entry * copy() {
			return new GUI_Add_Row_Entry(*this);
		}
	} GUI_Add_Row_Entry;

	/**
	 * \brief sets size of Widget
	 */
	typedef struct GUI_Size : GUIUpdateMessageStruct {
		double width, height;
		GUI_Size() {}
		GUI_Size(const std::string & name, const double w, const double h) : GUIUpdateMessageStruct(name), width(w), height(h) {
		}
		GUI_Size * copy() {
			return new GUI_Size(*this);
		}
	} GUI_Size;

	/**
	 * \brief creates basic GUI
	 */
	typedef struct GUI_Add_Create : GUIUpdateMessageStruct {
		std::string scheme, font, defFont, defMouseImageSet, defMouseImageName;
		GUI_Add_Create() {
		}
		GUI_Add_Create(const std::string & s, const std::string & f, const std::string & df, const std::string & dmis, const std::string & dmin);
		GUI_Add_Create * copy() {
			return new GUI_Add_Create(*this);
		}
	} GUI_Add_Create;

	/**
	 * \brief adds a window to root
	 */
	typedef struct GUI_AddToRoot_Update : GUIUpdateMessageStruct {
		std::string child;
		GUI_AddToRoot_Update() {
		}
		GUI_AddToRoot_Update(const std::string & c);
		GUI_AddToRoot_Update * copy() {
			return new GUI_AddToRoot_Update(*this);
		}
	} GUI_AddToRoot_Update;

	/**
	 * \brief updates mouse visibility
	 */
	typedef struct GUI_MouseVisible_Update : GUIUpdateMessageStruct {
		bool visible;
		GUI_MouseVisible_Update() {
		}
		GUI_MouseVisible_Update(const bool b);
		GUI_MouseVisible_Update * copy() {
			return new GUI_MouseVisible_Update(*this);
		}
	} GUI_MouseVisible_Update;

	/**
	 * \brief updates a child
	 */
	typedef struct GUI_Child_Update : GUIUpdateMessageStruct {
		std::string name;
		std::string child;
		GUI_Child_Update() {
		}
		GUI_Child_Update(const std::string & n, const std::string & c);
		GUI_Child_Update * copy() {
			return new GUI_Child_Update(*this);
		}
	} GUI_Child_Update;

	/**
	 * \brief cleans up GUI
	 */
	typedef struct GUI_CleanUp_Delete : GUIUpdateMessageStruct {
		GUI_CleanUp_Delete * copy() {
			return new GUI_CleanUp_Delete(*this);
		}
	} GUI_CleanUp_Delete;

	/**
	 * \brief subscribes method to be called using button
	 */
	typedef struct GUI_SubscribeEvent_Update : GUIUpdateMessageStruct {
		std::string name;
		std::string event;
		boost::function<void(void)> func;
		GUI_SubscribeEvent_Update() {
		}
		GUI_SubscribeEvent_Update(const std::string & n, const std::string & e, const boost::function<void(void)> & f);
		GUI_SubscribeEvent_Update * copy() {
			return new GUI_SubscribeEvent_Update(*this);
		}
	} GUI_SubscribeEvent_Update;

	/**
	 * \brief changes state of the event
	 */
	typedef struct GUI_Event_Update : GUIUpdateMessageStruct {
		std::string name;
		bool enabled;
		GUI_Event_Update() {}
		GUI_Event_Update(const std::string & n, bool b);
		GUI_Event_Update * copy() { return new GUI_Event_Update(*this); }
	} GUI_Event_Update;

	/**
	 * \brief registers a template for a Widget
	 */
	typedef struct GUI_WidgetTemplate_Create : GUIUpdateMessageStruct {
		std::string name;
		boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> func;
		GUI_WidgetTemplate_Create() {}
		GUI_WidgetTemplate_Create(const std::string & n, const boost::function<api::GUIWidget * (const std::string & name, const std::string & style)> & f);
		GUI_WidgetTemplate_Create * copy() { return new GUI_WidgetTemplate_Create(*this); }
	} GUI_WidgetTemplate_Create;

	/**
	 * \brief registers a RenderWindow in GUI subsystem
	 */
	typedef struct GUI_Window_Create : GUIUpdateMessageStruct {
		void * window;
		GUI_Window_Create(void * w);
		GUI_Window_Create * copy() { return new GUI_Window_Create(*this); }
	} GUI_Window_Create;

	/**
	 * \brief changes the mouse cursor
	 */
	typedef struct GUI_MouseCursorImage_Update : GUIUpdateMessageStruct {
		std::string image;
		GUI_MouseCursorImage_Update(const std::string & i);
		GUI_MouseCursorImage_Update * copy() { return new GUI_MouseCursorImage_Update(*this); }
	} GUI_MouseCursorImage_Update;

	/**
	 * \brief updates resolution of render window
	 */
	typedef struct GUI_Resolution_Update : GUIUpdateMessageStruct {
		graphics::Resolution resolution;
		GUI_Resolution_Update(const graphics::Resolution & r);
		GUI_Resolution_Update * copy() {
			return new GUI_Resolution_Update(*this);
		}
	} GUI_Resolution_Update;

	/**
	 * \brief adds a new imageset
	 */
	typedef struct GUI_AddImageset_Create : GUIUpdateMessageStruct {
		std::string imageset;
		GUI_AddImageset_Create(const std::string & i);
		GUI_AddImageset_Create * copy() {
			return new GUI_AddImageset_Create(*this);
		}
	} GUI_AddImageset_Create;

	/**
	 * \brief sets alignment of Widget
	 */
	typedef struct GUI_Alignment : GUIUpdateMessageStruct {
		Alignment alignment;
		GUI_Alignment(const std::string & name, Alignment a) : GUIUpdateMessageStruct(name), alignment(a) {
		}
		GUI_Alignment * copy() {
			return new GUI_Alignment(*this);
		}
	} GUI_Alignment;

	/**
	 * \brief enables or disables a GUIWidget as drop target
	 */
	typedef struct GUI_SetDropTarget : GUIUpdateMessageStruct {
		bool dropable;
		GUI_SetDropTarget(const std::string & name, bool d) : GUIUpdateMessageStruct(name), dropable(d) {
		}
		GUI_SetDropTarget * copy() {
			return new GUI_SetDropTarget(*this);
		}
	} GUI_SetDropTarget;

	/**
	 * \brief enables or disables dragging of Widget
	 */
	typedef struct GUI_SetDragable : GUIUpdateMessageStruct {
		bool dragable;
		GUI_SetDragable(const std::string & name, bool d) : GUIUpdateMessageStruct(name), dragable(d) {
		}
		GUI_SetDragable * copy() {
			return new GUI_SetDragable(*this);
		}
	} GUI_SetDragable;

	/**
	* \brief callback being called when Widget is dropped
	*/
	typedef struct GUI_SetDropCallback : GUIUpdateMessageStruct {
		std::function<void(const std::string &, const std::string &)> callback;
		GUI_SetDropCallback(const std::string & name, const std::function<void(const std::string &, const std::string &)> & c) : GUIUpdateMessageStruct(name), callback(c) {
		}
		GUI_SetDropCallback * copy() {
			return new GUI_SetDropCallback(*this);
		}
	} GUI_SetDropCallback;

} /* namespace gui */
} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_GUI_GUICONFIG_H__ */

/**
 * @}
 */
