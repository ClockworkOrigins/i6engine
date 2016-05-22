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

#include "DialogCreatorPythonScriptLanguage.h"

#include <iostream>

#include <QApplication>
#include <QDir>
#include <QQueue>
#include <QTextStream>

namespace i6e {
namespace plugins {

	void DialogCreatorPythonScriptLanguage::parseScripts(const QString & path) {
		QDir basePath = QDir(qApp->applicationDirPath() + "/" + path);
		QQueue<QDir> dirs;
		dirs.enqueue(basePath);
		while (!dirs.empty()) {
			QDir dir = dirs.dequeue();
			foreach(QString fileName, dir.entryList(QDir::Files)) {
				if (fileName.endsWith(".py")) {
					QFile f(dir.absoluteFilePath(fileName));
					if (f.open(QIODevice::ReadOnly)) {
						QTextStream in(&f);
						while (!in.atEnd()) {
							QString line = in.readLine();
							parseLine(line);
						}
						f.close();
					}
				}
			}
			foreach(QString dirName, dir.entryList(QDir::Dirs)) {
				if (dirName == "." || dirName == "..") {
					continue;
				}
				dirs.enqueue(dir.absolutePath() + "/" + dirName);
			}
		}
		std::cout << "Parsed " << _functions.size() << " python scripts" << std::endl;
	}

	QVector<dialogCreator::plugins::ScriptFunction> DialogCreatorPythonScriptLanguage::getScriptFunctions() const {
		return _functions;
	}

	void DialogCreatorPythonScriptLanguage::exportScript(const QVector<dialogCreator::plugins::i6Script> &) {
	}

	void DialogCreatorPythonScriptLanguage::parseLine(const QString & line) {
		if (line.contains("# i6Function: ")) {
			std::cout << "Checking line " << line.toStdString() << std::endl;
			QRegExp regex("[A-Za-z]+ [A-Za-z]+\\([(A-Za-z)+\\,?]+\\)");
			int pos = regex.indexIn(line);
			if (pos == -1) {
				std::cout << "Error" << std::endl;
				return;
			}
			QString i6Func = line.mid(pos, regex.matchedLength());
			regex = QRegExp("[A-Za-z]+");
			pos = regex.indexIn(i6Func);
			int length = regex.matchedLength();
			QString returnType = i6Func.mid(pos, length);
			pos += length + 1;
			pos = regex.indexIn(i6Func, pos);
			length = regex.matchedLength();
			QString name = i6Func.mid(pos, length);
			pos += length;

			dialogCreator::plugins::ScriptFunction scriptFunc;
			scriptFunc.name = name;
			scriptFunc.returnType = returnType;

			while ((pos = regex.indexIn(i6Func, pos)) != -1) {
				length = regex.matchedLength();
				QString parameterType = i6Func.mid(pos, length);
				pos += length;
				scriptFunc.signatureTypes.push_back(parameterType);
			}
			_functions.push_back(scriptFunc);
		}
	}

} /* namespace plugins */
} /* namespace i6e */

Q_PLUGIN_METADATA(IID "i6e::dialogCreator::plugins::ScriptLanguagePluginInterface")
