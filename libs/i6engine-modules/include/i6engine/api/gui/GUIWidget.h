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

#ifndef __I6ENGINE_API_GUIWIDGET_H__
#define __I6ENGINE_API_GUIWIDGET_H__

#include <vector>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/math/i6eVector2.h"

namespace CEGUI {
	class AnimationInstance;
	class EventArgs;
	class Window;
} /* namespace CEGUI */

namespace i6e {
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
		virtual void tick();

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
		void subscribeClickEvent(const std::function<void(void)> & callback) {
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

		/**
		 * \brief sets position of this widget in relative coordinates [0, 1]
		 */
		void setPosition(double x, double y);

		/**
		 * \brief sets position of this widget in absolute pixel coordinates
		 */
		void setPositionPxl(double x, double y);

		/**
		 * \brief sets size of this widget in relative size [0, 1]
		 */
		void setSize(double w, double h);

		/**
		 * \brief sets size of this widget in absolute pixel sizes
		 */
		void setSizePxl(double w, double h);
		
		bool isHit() const;

		//! handler to copy rendered data to a memory buffer
		bool renderingEndedHandler(const CEGUI::EventArgs & args);

		std::string _name;
		CEGUI::Window * _window;

		GUIWidget * _parent;
		std::vector<GUIWidget *> _childs;

	private:
		bool _ticking;
		std::function<std::vector<GUIWidget *>()> _mouseOverCallback;
		bool _dropable;
		std::function<bool(std::string)> _canDrop;
		bool _dragable;
		std::function<void(const std::string &, const std::string &)> _dropCallback;
		Vec2f _originalPos;
		Vec2f _originalPosPxl;
		bool _isDragged;
		Vec2f _dragOffset;
		std::function<void(void)> _clickCallback;
		std::function<void(void)> _mouseEnterCallback;
		std::function<void(void)> _mouseLeaveCallback;
		std::string _tooltip;
		bool _tooltipActive;
		std::map<std::string, CEGUI::AnimationInstance *> _animations;

		//! Pointer to buffer holding the render data
		uint32_t * _hitTestBuffer;
		//! Size of the hit test buffer (i.e. its capacity)
		size_t _hitBufferCapacity;
		//! Dimensions in pixels of the data in the hit test buffer
		Vec2f _hitBufferSize;
		//! whether data in hit test buffer is inverted.
		bool _hitBufferInverted;

		bool _transparencyCheckEnabled;

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
} /* namespace i6e */

#endif /* __I6ENGINE_API_GUIWIDGET_H__ */

/**
 * @}
 */
