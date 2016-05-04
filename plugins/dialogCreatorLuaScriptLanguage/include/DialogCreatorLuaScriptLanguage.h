#ifndef __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__
#define __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__

#include "ScriptLanguagePluginInterface.h"

#include <QObject>

namespace i6e {
namespace plugins {

	class DialogCreatorLuaScriptLanguage : public QObject, public dialogCreator::plugins::ScriptLanguagePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6e::dialogCreator::plugins::ScriptLanguagePluginInterface")
		Q_INTERFACES(i6e::dialogCreator::plugins::ScriptLanguagePluginInterface)

	public:
		/**
		 * \brief returns an identifier for this plugin, also used as name for the tab
		 */
		QString getName() const {
			return "Lua";
		}

		/**
		 * \brief provides the entry in i6engine.ini used to identify the path to the dialog scripts for Lua language
		 */
		QString getScriptLanguageEntry() const {
			return "LuaScriptsPath";
		}

		/**
		 * \brief parses all lua scripts
		 */
		void parseScripts(const QString & path);

		/**
		 * \brief returns all parsed lua script functions
		 */
		QVector<dialogCreator::plugins::ScriptFunction> getScriptFunctions() const;

		/**
		 * \brief converts i6Script to lua and exports as file
		 */
		void exportScript(const QVector<dialogCreator::plugins::i6Script> & dialog);

	private:
		QVector<dialogCreator::plugins::ScriptFunction> _functions;

		void parseLine(const QString & line);
	};

} /* namespace plugins */
} /* namespace i6e */

#endif /* __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__ */
