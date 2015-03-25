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

#ifndef __I6ENGINE_API_GUIWIDGET_H__
#define __I6ENGINE_API_GUIWIDGET_H__

#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/math/i6eVector2.h"

#include "boost/function.hpp"

namespace CEGUI {
	class EventArgs;
	class Window;
} /* namespace CEGUI */

namespace i6engine {
namespace api {
namespace gui {
	struct GUIUpdateMessageStruct;
} /* namespace gui */

	/**
	 * \class GUIWidget
	 * \brief Superclass for all GUI widgets.
	 *
	 * A GUIWidget is a part of your GUI. Each GUIWidget has an unique name and handles one CEGUI Window.
	 * The GUIWidgets are managed by the GUIManager:
	 *
	 * - Each widget is automatically added to the root window.
	 * - The manager assumes that there is a valid CEGUI-window in the _window variable. So the variable must
	 * 		be filled in the c'tor of subclasses.
	 * - The constructor of a GUIWidget-subclass should take two Strings. One for the name of the widget and
	 * 		one for the CEGUI-window layout. While this is not mandatory, it makes it easier to register new
	 * 		widget templates.
	 * - GUIWidgets are created by a Factory. The registerGUIWidgetTemplate function in the GUIFacade allows to
	 * 		register a new widget to the system. If the constructor of a subclass follows the above point, you can
	 * 		use boost::factory to simply register the constructor of the subclass to the System.
	 */
	class ISIXE_MODULES_API GUIWidget {
	public:
		explicit GUIWidget(const std::string & name);
		virtual ~GUIWidget();

		/**
		 * \brief Method to process incoming messages.
		 */
		virtual void update(uint16_t type, gui::GUIUpdateMessageStruct * message);

		/**
		 * \brief This method is called every Tick, if ticking is enabled for this widget
		 */
		virtual void tick() {}

		/**
		 * \brief loads a Layout from a .layout file and stores it in _window
		 */
		void loadWindowLayout(const std::string & name, const std::string & filename);

		/**
		 * \brief Registers this widget to be ticked.
		 * \param enabled If true this widget's tick-method will be called every tick
		 */
		void enableTicking(bool enabled);

		/**
		 * \brief Subscribes to a window event.
		 * This function can be used to handle event-subscription-messages.
		 * \param callback The function to be executed.
		 */
		void subscribeClickEvent(const boost::function<void(void)> & callback) {
			_clickCallback = callback;
		}

		/**
		 * \brief returns whether the widget accepts drops or not
		 */
		bool canDrop() const {
			return _dropable;
		}

		/**
		 * \brief returns a vector containing all windows the mousecursor is currently over
		 */
		std::vector<GUIWidget *> getAllMouseoverWidgets() const {
			return _mouseOverCallback();
		}

		/**
		 * \brief sets mouse over callback
		 */
		void setMouseOverCallback(const std::function<std::vector<GUIWidget *>(void)> & func) {
			_mouseOverCallback = func;
		}

		void setPosition(double x, double y);
		void setSize(double w, double h);

		std::string _name;
		CEGUI::Window * _window;

	private:
		std::function<std::vector<GUIWidget *>()> _mouseOverCallback;
		bool _dropable;
		bool _dragable;
		std::function<void(const std::string &, const std::string &)> _dropCallback;
		Vec2f _originalPos;
		bool _isDragged;
		Vec2f _dragOffset;
		boost::function<void(void)> _clickCallback;
		std::string _tooltip;
		bool _tooltipActive;

		bool drag(const CEGUI::EventArgs & e);
		bool drop(const CEGUI::EventArgs & e);
		bool mouseMove(const CEGUI::EventArgs & e);
		bool mouseEnter(const CEGUI::EventArgs & e);
		bool mouseLeave(const CEGUI::EventArgs & e);

		/**
		 * \brief forbidden
		 */
		GUIWidget(const GUIWidget &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIWidget & operator=(const GUIWidget &) = delete;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_GUIWIDGET_H__ */

/**
 * @}
 */
