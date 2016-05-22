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

#ifndef __I6ENGINE_PLUGINS_EDITORLOADLEVELRPG_H__
#define __I6ENGINE_PLUGINS_EDITORLOADLEVELRPG_H__

#include "i6engine/editor/plugins/LoadLevelPluginInterface.h"

#include "i6engine/i6engineBuildSettings.h"

#if ISIXE_SCRIPTING == SCRIPTING_LUA
	#include "i6engine/modules/luascripting/LuaScriptingManager.h"
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
	#include "i6engine/modules/pythonscripting/PythonScriptingManager.h"
#endif

namespace i6e {
namespace plugins {

	class EditorLoadLevelRPG : public QObject, public editor::plugins::LoadLevelPluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e.editor.loadLevel.LoadLevelPluginInterface")
		Q_INTERFACES(i6e::editor::plugins::LoadLevelPluginInterface)

	public:
		EditorLoadLevelRPG();

		/**
		 * \brief called before loading the level file
		 */
		void startLoadLevel(const QString & level) override;

		/**
		 * \brief called after loading the complete level file
		 */
		void finishLoadLevel(const QString & level) override;

	private:
#if ISIXE_SCRIPTING == SCRIPTING_LUA
		modules::LuaScriptingManager _scriptingManager;
#elif ISIXE_SCRIPTING == SCRIPTING_PYTHON
		modules::PythonScriptingManager _scriptingManager;
#endif
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_EDITORLOADLEVELRPG_H__ */
