#include "DialogCreatorLuaScriptLanguage.h"

namespace i6engine {
namespace plugins {

	void DialogCreatorLuaScriptLanguage::parseScripts(const QString & path) {

	}

	QVector<dialogCreator::plugins::ScriptFunction> DialogCreatorLuaScriptLanguage::getScriptFunctions() const {
		return {};
	}

	void DialogCreatorLuaScriptLanguage::exportScript(const QVector<dialogCreator::plugins::i6Script> & dialog) {

	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface")
