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

#ifndef __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__
#define __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__

#include "i6engine/editor/plugins/SaveObjectPluginInterface.h"

#include <fstream>

namespace i6e {
namespace plugins {

	class EditorSaveObjectRPG : public QObject, public editor::plugins::SaveObjectPluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.saveObject.SaveObjectPluginInterface")
		Q_INTERFACES(i6e::editor::plugins::SaveObjectPluginInterface)

	public:
		EditorSaveObjectRPG();

		/**
		 * \brief initializes save process using given level file
		 */
		void startLevel(const QString & level);

		/**
		 * \brief tries to save GameObject during saving in editor
		 * \param[in] go the GOPtr to the GameObject to be saved
		 * \param[in] element the XMLElement this go should be linked to if necessary
		 * \returns true if object was handled by this plugin, otherwise false
		 */
		bool saveObject(const i6e::api::GOPtr & go, tinyxml2::XMLElement * element);

		/**
		 * \brief finishes save process
		 */
		void finishLevel();

	private:
		std::ofstream _file;
		QString _scriptsPath;
		QString _fileExtension;
		QString _level;

		void insertMethodDeclaration();
		void saveNPC(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);
		void saveItem(const std::string & identifier, const Vec3 & pos, const Quaternion & rot);
		void insertMethodEnd();
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORSAVEOBJECTRPG_H__ */
