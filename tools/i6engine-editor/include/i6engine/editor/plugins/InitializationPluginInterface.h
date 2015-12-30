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

#ifndef __I6ENGINE_EDITOR_PLUGINS_INITIALIZATIONPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_INITIALIZATIONPLUGININTERFACE_H__

#include <qplugin.h>

namespace i6engine {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins used to initialize stuff like Component factory methods
	 */
	class InitializationPluginInterface {
	public:
		virtual ~InitializationPluginInterface() {
		}

		virtual void initialize() = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6engine */

Q_DECLARE_INTERFACE(i6engine::editor::plugins::InitializationPluginInterface, "i6engine.editor.initialization.InitializationPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_INITIALIZATIONPLUGININTERFACE_H__ */
