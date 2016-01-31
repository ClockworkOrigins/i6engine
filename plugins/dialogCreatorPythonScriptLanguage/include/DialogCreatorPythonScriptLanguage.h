#ifndef __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__
#define __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__

#include "ScriptLanguagePluginInterface.h"

#include <QObject>

namespace i6engine {
namespace plugins {

	class DialogCreatorPythonScriptLanguage : public QObject, public dialogCreator::plugins::ScriptLanguagePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface")
		Q_INTERFACES(i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface)

	public:
		/**
		 * \brief provides the entry in DialogCreator.ini used to identify the path to the dialog scripts for python language
		 */
		QString getScriptLanguageEntry() const {
			return "PythonDialogScriptsPath";
		}

		/**
		 * \brief parses all python scripts
		 */
		void parseScripts(const QString & path);

		/**
		 * \brief returns all parsed python script functions
		 */
		QVector<dialogCreator::plugins::ScriptFunction> getScriptFunctions() const;

		/**
		 * \brief converts i6Script to python and exports as file
		 */
		void exportScript(const QVector<dialogCreator::plugins::i6Script> & dialog);
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__ */
