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
