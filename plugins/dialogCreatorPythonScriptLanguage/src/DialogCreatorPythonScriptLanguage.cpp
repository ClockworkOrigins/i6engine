#include "DialogCreatorPythonScriptLanguage.h"

namespace i6engine {
namespace plugins {

	void DialogCreatorPythonScriptLanguage::parseScripts(const QString & path) {

	}

	QVector<dialogCreator::plugins::ScriptFunction> DialogCreatorPythonScriptLanguage::getScriptFunctions() const {
		return {};
	}

	void DialogCreatorPythonScriptLanguage::exportScript(const QVector<dialogCreator::plugins::i6Script> & dialog) {

	}

} /* namespace plugins */
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface")
