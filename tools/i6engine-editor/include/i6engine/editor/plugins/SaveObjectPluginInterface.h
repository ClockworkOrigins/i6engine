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

#ifndef __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__

#include <qplugin.h>
#include <QWidget>

#include "i6engine/api/objects/GameObject.h"

namespace tinyxml2 {
	class XMLElement;
} /* namespace tinyxml2 */

namespace i6engine {
namespace editor {
namespace plugins {

	/**
	 * \brief interface for plugins to save special GameObjects in another way than normal GameObjects (e.g. NPCs of RPG are put into Startup method for the level instead of in the level file)
	 */
	class SaveObjectPluginInterface {
	public:
		virtual ~SaveObjectPluginInterface() {
		}

		/**
		 * \brief initializes save process using given level file
		 */
		virtual void startLevel(const QString & level) = 0;

		/**
		 * \brief tries to save GameObject during saving in editor
		 * \param[in] go the GOPtr to the GameObject to be saved
		 * \param[in] element the XMLElement this go should be linked to if necessary
		 * \returns true if object was handled by this plugin, otherwise false
		 */
		virtual bool saveObject(const i6engine::api::GOPtr & go, tinyxml2::XMLElement * element) = 0;

		/**
		 * \brief finishes save process
		 */
		virtual void finishLevel() = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6engine */

Q_DECLARE_INTERFACE(i6engine::editor::plugins::SaveObjectPluginInterface, "i6engine.editor.saveObject.SaveObjectPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__ */
