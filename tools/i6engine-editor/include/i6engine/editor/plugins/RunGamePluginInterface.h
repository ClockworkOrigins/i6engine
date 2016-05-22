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

#ifndef __I6ENGINE_EDITOR_PLUGINS_RUNGAMEPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_RUNGAMEPLUGININTERFACE_H__

#include "i6engine/api/Application.h"

#include <qplugin.h>

namespace i6e {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins used to run a game with a level
	 */
	class RunGamePluginInterface : public api::Application {
	public:
		virtual ~RunGamePluginInterface() {
		}

		/**
		 * \brief returns the string used for the entry in the game menu
		 */
		virtual QString getMenuEntry() const = 0;

		/**
		 * \brief initializes subsystems for this game
		 */
		virtual void initializeSubSystems(HWND hWnd) = 0;

		/**
		 * \brief called before engine is started, plugin has to ensure level is loaded!
		 */
		void setLevel(const std::string & level) {
			_level = level;
		}

		/**
		 * \brief callback is used to notify editor finalize is called
		 */
		void setFinalizeCallback(const std::function<void(void)> & callback) {
			_callback = callback;
		}

	protected:
		std::string _level;
		std::function<void(void)> _callback;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::RunGamePluginInterface, "i6e.editor.runGame.RunGamePluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_RUNGAMEPLUGININTERFACE_H__ */
