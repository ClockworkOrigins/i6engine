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
