#ifndef __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__
#define __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__

#include "ScriptLanguagePluginInterface.h"

#include <QObject>

namespace i6engine {
namespace plugins {

	class DialogCreatorLuaScriptLanguage : public QObject, public dialogCreator::plugins::ScriptLanguagePluginInterface {
		Q_OBJECT
		Q_PLUGIN_METADATA(IID "i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface")
		Q_INTERFACES(i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface)

	public:
		/**
		 * \brief provides the entry in DialogCreator.ini used to identify the path to the dialog scripts for Lua language
		 */
		QString getScriptLanguageEntry() const {
			return "LuaDialogScriptsPath";
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
	};

} /* namespace plugins */
} /* namespace i6engine */

#endif /* __I6ENGINE_PLUGINS_DIALOGCREATORLUASCRIPTLANGUAGE_H__ */
