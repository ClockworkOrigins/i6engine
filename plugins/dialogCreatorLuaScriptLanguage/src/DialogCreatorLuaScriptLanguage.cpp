#include "DialogCreatorLuaScriptLanguage.h"

#include <iostream>

#include <QApplication>
#include <QDir>
#include <QQueue>
#include <QTextStream>

namespace i6engine {
namespace plugins {

	void DialogCreatorLuaScriptLanguage::parseScripts(const QString & path) {
		QDir basePath = QDir(qApp->applicationDirPath() + "/" + path);
		QQueue<QDir> dirs;
		dirs.enqueue(basePath);
		while (!dirs.empty()) {
			QDir dir = dirs.dequeue();
			foreach(QString fileName, dir.entryList(QDir::Files)) {
				if (fileName.endsWith(".lua")) {
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
		std::cout << "Parsed " << _functions.size() << " lua scripts" << std::endl;
	}

	QVector<dialogCreator::plugins::ScriptFunction> DialogCreatorLuaScriptLanguage::getScriptFunctions() const {
		return _functions;
	}

	void DialogCreatorLuaScriptLanguage::exportScript(const QVector<dialogCreator::plugins::i6Script> &) {
	}

	void DialogCreatorLuaScriptLanguage::parseLine(const QString & line) {
		if (line.contains("-- i6Function: ")) {
			QRegExp regex("[A-Za-z]+ [A-Za-z]+\\([(A-Za-z)+\\,?]+\\)");
			int pos = regex.indexIn(line);
			if (pos == -1) {
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
} /* namespace i6engine */

Q_PLUGIN_METADATA(IID "i6engine::dialogCreator::plugins::ScriptLanguagePluginInterface")
