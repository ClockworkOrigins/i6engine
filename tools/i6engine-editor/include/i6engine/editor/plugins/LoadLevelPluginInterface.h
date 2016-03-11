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

#ifndef __I6ENGINE_EDITOR_PLUGINS_LOADLEVELPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_LOADLEVELPLUGININTERFACE_H__

#include <qplugin.h>

namespace i6engine {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins to load other stuff in editor besides the level file (e.g. NPCs from Startup script in RPG)
	 */
	class LoadLevelPluginInterface {
	public:
		virtual ~LoadLevelPluginInterface() {
		}

		/**
		 * \brief called before loading the level file
		 */
		virtual void startLoadLevel(const QString & level) = 0;

		/**
		 * \brief called after loading the complete level file
		 */
		virtual void finishLoadLevel(const QString & level) = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6engine */

Q_DECLARE_INTERFACE(i6engine::editor::plugins::LoadLevelPluginInterface, "i6engine.editor.loadLevel.LoadLevelPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_LOADLEVELPLUGININTERFACE_H__ */
