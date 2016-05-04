#ifndef __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__
#define __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__

#include "ScriptLanguagePluginInterface.h"

#include <QObject>

namespace i6e {
namespace plugins {

	class DialogCreatorPythonScriptLanguage : public QObject, public dialogCreator::plugins::ScriptLanguagePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e::dialogCreator::plugins::ScriptLanguagePluginInterface")
		Q_INTERFACES(i6e::dialogCreator::plugins::ScriptLanguagePluginInterface)

	public:
		/**
		 * \brief returns an identifier for this plugin, also used as name for the tab
		 */
		QString getName() const {
			return "Python";
		}
		/**
		 * \brief provides the entry in i6engine.ini used to identify the path to the dialog scripts for python language
		 */
		QString getScriptLanguageEntry() const {
			return "PythonScriptsPath";
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

	private:
		QVector<dialogCreator::plugins::ScriptFunction> _functions;

		void parseLine(const QString & line);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_DIALOGCREATORPYTHONSCRIPTLANGUAGE_H__ */
