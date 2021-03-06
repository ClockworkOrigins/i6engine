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

#include "widgets/DialogHeaderWidget.h"

#include "i6engine/api/EngineController.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "tinyxml2.h"

namespace i6e {
namespace dialogCreator {
namespace widgets {

	DialogHeaderWidget::DialogHeaderWidget(QWidget * par) : QWidget(par), _participants(), _dialogDirectory(), _pythonScriptsPath(), _luaScriptsPath() {
		setupUi(this);

		_participants.push_back(new QLineEdit(this));
		participantsLayout->addWidget(_participants.back());
		connect(_participants.back(), SIGNAL(textChanged(const QString &)), this, SLOT(updatedParticipants()));

		pythonConditionTextEdit->setVisible(false);
		luaConditionTextEdit->setVisible(false);

		connect(conditionLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateConditionEntry()));
		connect(informationLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateInfoEntry()));
		
		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("DialogCreator.ini")) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Error during startup!"));
			box.setInformativeText(QApplication::tr("DialogCreator.ini not found!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
		}
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("SCRIPT", "PythonDialogScriptsPath", _pythonScriptsPath)) {
			_pythonScriptsPath = "";
			pythonInfoTextEdit->setVisible(false);
		}
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("SCRIPT", "LuaDialogScriptsPath", _luaScriptsPath)) {
			_luaScriptsPath = "";
			luaInfoTextEdit->setVisible(false);
		}

		identifierLabel->setText(QApplication::tr("Identifier"));
		participantsLabel->setText(QApplication::tr("Participants"));
		numberLabel->setText(QApplication::tr("Number"));
		permanentCheckBox->setText(QApplication::tr("Permanent"));
		importantCheckBox->setText(QApplication::tr("Important"));
		descriptionLabel->setText(QApplication::tr("Description"));
		conditionLabel->setText(QApplication::tr("Condition Script"));
		informationLabel->setText(QApplication::tr("Information Script"));
		saveChangesButton->setText(QApplication::tr("Save changes"));
		saveNewDialogButton->setText(QApplication::tr("Save new dialog"));
	}

	DialogHeaderWidget::~DialogHeaderWidget() {
	}

	void DialogHeaderWidget::showDialog(QString identifier) {
		while (_participants.size() > 1) {
			_participants.front()->setText("");
		}
		rpg::dialog::Dialog * dialog = rpg::dialog::DialogManager::GetSingletonPtr()->getDialogs()[identifier.toStdString()];
		identifierLineEdit->setText(QString::fromStdString(dialog->identifier));
		for (std::string s : dialog->participants) {
			_participants.back()->setText(QString::fromStdString(s));
		}
		numberSpinBox->setValue(int(dialog->nr));
		permanentCheckBox->setChecked(dialog->permanent);
		importantCheckBox->setChecked(dialog->important);
		descriptionLineEdit->setText(QString::fromStdString(dialog->description));
		conditionLineEdit->setText(QString::fromStdString(dialog->conditionScript));
		informationLineEdit->setText(QString::fromStdString(dialog->infoScript));
	}

	void DialogHeaderWidget::updateConditionEntry() {
		if (conditionLineEdit->text().isEmpty()) {
			pythonConditionTextEdit->setVisible(false);
			luaConditionTextEdit->setVisible(false);
		} else {
			pythonConditionTextEdit->setVisible(!_pythonScriptsPath.empty());
			luaConditionTextEdit->setVisible(!_luaScriptsPath.empty());
			if (!_pythonScriptsPath.empty()) {
				QFile f(QString::fromStdString(_pythonScriptsPath) + "/" + conditionLineEdit->text() + ".py");
				if (f.open(QFile::ReadOnly | QFile::Text)) {
					QTextStream in(&f);
					pythonConditionTextEdit->setText(in.readAll());
				} else {
					pythonConditionTextEdit->setText(QString("def ") + conditionLineEdit->text() + QString("():\n\treturn True"));
				}
			}
			if (!_luaScriptsPath.empty()) {
				QFile f(QString::fromStdString(_luaScriptsPath) + "/" + conditionLineEdit->text() + ".lua");
				if (f.open(QFile::ReadOnly | QFile::Text)) {
					QTextStream in(&f);
					luaConditionTextEdit->setText(in.readAll());
				} else {
					luaConditionTextEdit->setText(QString("function ") + conditionLineEdit->text() + QString("()\n\treturn true\nend"));
				}
			}
		}
	}

	void DialogHeaderWidget::updateInfoEntry() {
		if (!_pythonScriptsPath.empty()) {
			QFile f(QString::fromStdString(_pythonScriptsPath) + "/" + informationLineEdit->text() + ".py");
			if (f.open(QFile::ReadOnly | QFile::Text)) {
				QTextStream in(&f);
				pythonInfoTextEdit->setText(in.readAll());
			} else {
				pythonInfoTextEdit->setText(QString("def ") + informationLineEdit->text() + QString("():\n\treturn"));
			}
		}
		if (!_luaScriptsPath.empty()) {
			QFile f(QString::fromStdString(_luaScriptsPath) + "/" + informationLineEdit->text() + ".lua");
			if (f.open(QFile::ReadOnly | QFile::Text)) {
				QTextStream in(&f);
				luaInfoTextEdit->setText(in.readAll());
			} else {
				luaInfoTextEdit->setText(QString("function ") + informationLineEdit->text() + QString("()\nend"));
			}
		}
	}

	void DialogHeaderWidget::updatedParticipants() {
		for (size_t i = 0; i < _participants.size(); i++) {
			if (i < _participants.size() - 1) {
				if (_participants[i]->text().isEmpty()) {
					delete _participants[i];
					_participants.erase(_participants.begin() + int(i));
					break;
				}
			} else {
				if (!_participants[i]->text().isEmpty()) {
					_participants.push_back(new QLineEdit(this));
					participantsLayout->addWidget(_participants.back());
					connect(_participants.back(), SIGNAL(textChanged(const QString &)), this, SLOT(updatedParticipants()));
				}
			}
		}
	}

	void DialogHeaderWidget::saveChanges() {
		if (identifierLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Incomplete data!"));
			box.setInformativeText(QApplication::tr("Identifier for dialog not set!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (_participants.size() == 1) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Incomplete data!"));
			box.setInformativeText(QApplication::tr("A dialog has to have at least one participant!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (!importantCheckBox->isChecked() && descriptionLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Incomplete data!"));
			box.setInformativeText(QApplication::tr("An unimportant dialog has to have a description!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (informationLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setWindowTitle(QApplication::tr("Incomplete data!"));
			box.setInformativeText(QApplication::tr("Information script required!"));
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration * decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.InsertFirstChild(decl);

		tinyxml2::XMLElement * dialog = doc.NewElement("Dialog");

		dialog->SetAttribute("identifier", identifierLineEdit->text().toStdString().c_str());

		tinyxml2::XMLElement * participant = doc.NewElement("Participants");

		std::string participants;

		for (size_t i = 0; i < _participants.size() - 1; i++) {
			participants += _participants[i]->text().toStdString() + ";";
		}

		participant->InsertFirstChild(doc.NewText(participants.c_str()));
		dialog->InsertEndChild(participant);

		tinyxml2::XMLElement * number = doc.NewElement("Nr");
		number->InsertFirstChild(doc.NewText(std::to_string(numberSpinBox->value()).c_str()));
		dialog->InsertEndChild(number);

		if (permanentCheckBox->isChecked()) {
			tinyxml2::XMLElement * permanent = doc.NewElement("Permanent");
			permanent->InsertFirstChild(doc.NewText("True"));
			dialog->InsertEndChild(permanent);
		}
		if (importantCheckBox->isChecked()) {
			tinyxml2::XMLElement * important = doc.NewElement("Important");
			important->InsertFirstChild(doc.NewText("True"));
			dialog->InsertEndChild(important);
		}
		if (!importantCheckBox->isChecked()) {
			tinyxml2::XMLElement * description = doc.NewElement("Description");
			description->InsertFirstChild(doc.NewText(descriptionLineEdit->text().toStdString().c_str()));
			dialog->InsertEndChild(description);
		}
		if (!conditionLineEdit->text().isEmpty()) {
			tinyxml2::XMLElement * condition = doc.NewElement("Condition");
			condition->InsertFirstChild(doc.NewText(conditionLineEdit->text().toStdString().c_str()));
			dialog->InsertEndChild(condition);
		}
		tinyxml2::XMLElement * information = doc.NewElement("Info");
		information->InsertFirstChild(doc.NewText(informationLineEdit->text().toStdString().c_str()));
		dialog->InsertEndChild(information);

		doc.InsertAfterChild(decl, dialog);

		doc.SaveFile((_dialogDirectory + "/" + identifierLineEdit->text().toStdString() + ".xml").c_str());

		if (!_pythonScriptsPath.empty()) {
			{
				QFile f(QString::fromStdString(_pythonScriptsPath) + "/" + informationLineEdit->text() + ".py");
				f.open(QFile::WriteOnly | QFile::Text);
				QTextStream out(&f);
				out << pythonInfoTextEdit->toPlainText();
			}
			{
				if (!conditionLineEdit->text().isEmpty()) {
					QFile f(QString::fromStdString(_pythonScriptsPath) + "/" + conditionLineEdit->text() + ".py");
					f.open(QFile::WriteOnly | QFile::Text);
					QTextStream out(&f);
					out << pythonConditionTextEdit->toPlainText();
				}
			}
		}
		if (!_luaScriptsPath.empty()) {
			{
				QFile f(QString::fromStdString(_luaScriptsPath) + "/" + informationLineEdit->text() + ".lua");
				f.open(QFile::WriteOnly | QFile::Text);
				QTextStream out(&f);
				out << luaInfoTextEdit->toPlainText();
			}
			{
				if (!conditionLineEdit->text().isEmpty()) {
					QFile f(QString::fromStdString(_luaScriptsPath) + "/" + conditionLineEdit->text() + ".lua");
					f.open(QFile::WriteOnly | QFile::Text);
					QTextStream out(&f);
					out << luaConditionTextEdit->toPlainText();
				}
			}
		}

		emit refreshLists();
	}

	void DialogHeaderWidget::saveNewDialog() {
		saveChanges();
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6e */
