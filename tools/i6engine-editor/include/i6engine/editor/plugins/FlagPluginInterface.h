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

#ifndef __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__

#include <string>
#include <vector>

#include <qplugin.h>

namespace i6e {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins used to offer flags for maps
	 */
	class FlagPluginInterface {
	public:
		virtual ~FlagPluginInterface() {
		}

		virtual std::vector<std::string> getFlags() const = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::FlagPluginInterface, "i6e.editor.initialization.FlagPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_FLAGPLUGININTERFACE_H__ */
