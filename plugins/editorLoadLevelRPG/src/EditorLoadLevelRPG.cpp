#include "EditorLoadLevelRPG.h"

#include <QStringList>

namespace i6e {
namespace plugins {

	EditorLoadLevelRPG::EditorLoadLevelRPG() : _scriptingManager() {
		_scriptingManager.loadAllScripts();
	}

	void EditorLoadLevelRPG::startLoadLevel(const QString &) {
	}

	void EditorLoadLevelRPG::finishLoadLevel(const QString & level) {
		QStringList vec = level.split("/");
		vec = vec.back().split("\\");
		QString levelStartup = vec.back().split(".").front();
		_scriptingManager.callFunctionWithCallback<void>(("Startup_" + levelStartup).toStdString(), []() {
		});
		_scriptingManager.Tick();
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6engine.editor.loadLevel.LoadLevelPluginInterface")
