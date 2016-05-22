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

#ifndef __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__
#define __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__

#include <qplugin.h>
#include <QStringList>
#include <QVector>

namespace i6e {
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
} /* namespace i6e */

Q_DECLARE_INTERFACE(i6e::dialogCreator::plugins::ScriptLanguagePluginInterface, "i6e.dialogCreator.script.ScriptLanguagePluginInterface")

#endif /* __DIALOGCREATOR_PLUGINS_SCRIPTLANGUAGEPLUGININTERFACE_H__ */
