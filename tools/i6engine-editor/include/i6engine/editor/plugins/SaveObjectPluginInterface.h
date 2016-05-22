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

#ifndef __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__
#define __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__

#include <qplugin.h>
#include <QWidget>

#include "i6engine/api/objects/GameObject.h"

namespace tinyxml2 {
	class XMLElement;
} /* namespace tinyxml2 */

namespace i6e {
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
		virtual bool saveObject(const i6e::api::GOPtr & go, tinyxml2::XMLElement * element) = 0;

		/**
		 * \brief finishes save process
		 */
		virtual void finishLevel() = 0;
	};

} /* namespace plugins */
} /* namespace editor */
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::editor::plugins::SaveObjectPluginInterface, "i6e.editor.saveObject.SaveObjectPluginInterface")

#endif /* __I6ENGINE_EDITOR_PLUGINS_SAVEOBJECTPLUGININTERFACE_H__ */
