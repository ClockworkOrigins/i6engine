/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__
#define __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__

#include <qplugin.h>
#include <QStringList>
#include <QVector>

namespace i6engine {
namespace dialogCreator {
namespace plugins {

	struct ScriptFunction {
		QString name;
		QString returnType;
		QStringList signatureTypes;
	};

	struct i6Script {
		ScriptFunction function;
		QString returnName;
		QStringList signatureNames;
	};

	/**
	 * \brief interface for plugins used to handle scripts
	 * this means parsing of scripts for annotations, offering exported engine functions and converting i6DialogLanguage to the script language
	 */
	class ScriptLanguagePluginInterface {
	public:
		virtual ~ScriptLanguagePluginInterface() {
		}

		/**
		 * \brief returns an identifier for this plugin, also used as name for the tab
		 */
		virtual QString getName() const = 0;

		/**
		 * \brief every script language supported by a plugin needs an entry in i6engine.ini to read the path of the scripts and this method provies this entry
		 */
		virtual QString getScriptLanguageEntry() const = 0;

		/**
		 * \brief parses all scripts for this plugin in the specified path
		 */
		virtual void parseScripts(const QString & path) = 0;

		/**
		 * \brief returns all parsed script functions
		 */
		virtual QVector<ScriptFunction> getScriptFunctions() const = 0;

		/**
		 * \brief converts i6Scripts to the scripting language and exports as file
		 */
		virtual void exportScript(const QVector<i6Script> & dialog) = 0;
	};

} /* namespace plugins */
} /* namespace dialogCreator */
} /* namespace i6engine */

Q_DECLARE_INTERFACE(i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface, "i6engine.dialogCreator.script.ScriptLanguagePluginInterface")

#endif /* __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__ */
