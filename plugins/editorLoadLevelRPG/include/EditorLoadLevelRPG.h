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
