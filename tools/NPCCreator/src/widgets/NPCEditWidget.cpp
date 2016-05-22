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

#include "widgets/NPCEditWidget.h"

#include "i6engine/api/EngineController.h"
#include "i6engine/api/components/MeshAppearanceComponent.h"
#include "i6engine/api/configs/ComponentConfig.h"
#include "i6engine/api/facades/ObjectFacade.h"
#include "i6engine/api/objects/GameObject.h"

#include "i6engine/rpg/npc/NPCManager.h"

#include "widgets/MainWindow.h"
#include "widgets/NPCListWidget.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreResourceManager.h"

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

#include "tinyxml2.h"

namespace i6e {
namespace tools {
namespace npcCreator {
namespace widgets {

	NPCEditWidget::NPCEditWidget(QWidget * par) : QWidget(par), _parent(dynamic_cast<MainWindow *>(par)), _npcListWidget(nullptr), _changed(false), _changedIdentifier(false), _windowTitle(_parent->windowTitle()), _currentIdentifier(), _currentFile(), _npcFileList() {
		setupUi(this);

		connect(identifierLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(changedIdentifier()));
		connect(nameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(changedEntry()));
		connect(currentXPSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedEntry()));
		connect(nextXPSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedEntry()));
		connect(levelSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedEntry()));
		connect(currentHPSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedEntry()));
		connect(maxHPSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedEntry()));

		identifierLabel->setText(QApplication::tr("Identifier"));
		identifierLabel->setText(QApplication::tr("Name"));
		identifierLabel->setText(QApplication::tr("Current XP"));
		identifierLabel->setText(QApplication::tr("Next XP"));
		identifierLabel->setText(QApplication::tr("Level"));
		identifierLabel->setText(QApplication::tr("Hitpoints"));
		identifierLabel->setText(QApplication::tr("Model"));
		identifierLabel->setText(QApplication::tr("Delete NPC"));
		identifierLabel->setText(QApplication::tr("Save changes"));
		identifierLabel->setText(QApplication::tr("Save new NPC"));
	}

	NPCEditWidget::~NPCEditWidget() {
	}

	void NPCEditWidget::changedIdentifier() {
		changedEntry();
		_changedIdentifier = true;
	}

	void NPCEditWidget::changedEntry() {
		if (!_changed) {
			_parent->setWindowTitle(_windowTitle + " *");
		}
		_changed = true;
	}

	void NPCEditWidget::saveNewNPC() {
		for (auto & p : _npcFileList) {
			if (QString::fromStdString(p.first) == identifierLineEdit->text()) {
				QMessageBox box;
				box.setText(QApplication::tr("Identifier already in use! Choose another one!"));
				box.exec();
				return;
			}
		}
		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QApplication::tr("RPG.ini not found!"));
			box.exec();
		}
		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QApplication::tr("No entry for npcDirectory in RPG.ini!"));
			box.exec();
		}
		saveToFile(NPCDirectory + "/" + identifierLineEdit->text().toStdString() + ".xml");
	}

	void NPCEditWidget::saveChanges() {
		if (!_changed) {
			QMessageBox box;
			box.setText(QApplication::tr("No changes to save!"));
			box.exec();
			return;
		}
		if (_changedIdentifier) {
			QMessageBox box;
			box.setText(QApplication::tr("Can't save changes when identifier was changed! Save as new NPC!"));
			box.exec();
			return;
		}
		saveToFile(_currentFile);
	}

	void NPCEditWidget::deleteNPC() {
		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QApplication::tr("RPG.ini not found!"));
			box.exec();
			return;
		}
		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QApplication::tr("No entry for npcDirectory in RPG.ini!"));
			box.exec();
			return;
		}
		QFile f(qApp->applicationDirPath() + "/" + QString::fromStdString(NPCDirectory) + "/" + identifierLineEdit->text() + ".xml");
		if (f.exists()) {
			f.remove();
		}
		_npcListWidget->refreshNPCList();
	}

	void NPCEditWidget::selectModel() {
		QStringList meshList;
		Ogre::FileInfoListPtr fileinfoiter = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("i6engine");
		for (unsigned int i = 0; i < (*fileinfoiter).size(); i++) {
			if (QString::fromStdString((*fileinfoiter)[i].filename).contains(".mesh", Qt::CaseInsensitive)) {
				meshList.push_back(QString::fromStdString((*fileinfoiter)[i].filename));
			}
		}
		QInputDialog dlg(this);
		dlg.setComboBoxEditable(false);
		dlg.setComboBoxItems(meshList);
		dlg.setWindowTitle(QApplication::tr("Select Model"));
		dlg.setLabelText(QApplication::tr("Select a model:"));
		dlg.setInputMode(QInputDialog::InputMode::TextInput);
		if (QDialog::Accepted == dlg.exec()) {
			modelLineEdit->setText(dlg.textValue());
			auto goList = api::EngineController::GetSingleton().getObjectFacade()->getAllObjectsOfType("NPC");
			if (goList.size() > 0) {
				goList[0]->getGOC<api::MeshAppearanceComponent>(api::components::ComponentTypes::MeshAppearanceComponent)->setMesh(dlg.textValue().toStdString());
			}
		}
	}

	void NPCEditWidget::parseNPC(const std::vector<std::pair<std::string, std::string>> & npcFileList, int index) {
		// start with removing of old NPC, can be done in parallel to the rest
		api::EngineController::GetSingleton().getObjectFacade()->deleteAllObjectsOfType("NPC");

		std::string file = npcFileList[index].second;
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile(file.c_str())) {
			QMessageBox box;
			box.setText(QApplication::tr("Couldn't open NPC file (") + QString::fromStdString(file) + QString(")"));
			box.exec();
			return;
		}

		for (tinyxml2::XMLElement * npc = doc.FirstChildElement("NPC"); npc != nullptr; npc = npc->NextSiblingElement("NPC")) {
			if (npc->Attribute("identifier") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses identifier!"));
				box.exec();
				return;
			}
			QString identifier(npc->Attribute("identifier"));
			if (QString::fromStdString(npcFileList[index].first) != identifier) {
				continue;
			}
			if (npc->FirstChildElement("Name") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses Name!"));
				box.exec();
				return;
			}
			QString name(npc->FirstChildElement("Name")->GetText());
			if (npc->FirstChildElement("XP") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses XP!"));
				box.exec();
				return;
			}
			int xp = std::stoi(npc->FirstChildElement("XP")->GetText());
			if (npc->FirstChildElement("NextXP") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses NextXP!"));
				box.exec();
				return;
			}
			int nextXP = std::stoi(npc->FirstChildElement("NextXP")->GetText());
			if (npc->FirstChildElement("Level") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses Level!"));
				box.exec();
				return;
			}
			int level = std::stoi(npc->FirstChildElement("Level")->GetText());
			if (npc->FirstChildElement("Model") == nullptr) {
				QMessageBox box;
				box.setText(QApplication::tr("NPC in file '") + QString::fromStdString(file) + QApplication::tr("' misses Model!"));
				box.exec();
				return;
			}
			QString model = npc->FirstChildElement("Model")->GetText();

			identifierLineEdit->setText(identifier);
			nameLineEdit->setText(name);
			currentXPSpinBox->setValue(xp);
			nextXPSpinBox->setValue(nextXP);
			levelSpinBox->setValue(level);
			modelLineEdit->setText(model);

			// optional stuff
			// attributes
			if (npc->FirstChildElement("HP") != nullptr) {
				int hp = std::stoi(npc->FirstChildElement("HP")->GetText());
				currentHPSpinBox->setValue(hp);
			}
			if (npc->FirstChildElement("MaxHP") != nullptr) {
				int maxHP = std::stoi(npc->FirstChildElement("MaxHP")->GetText());
				maxHPSpinBox->setValue(maxHP);
			}
		}
		_parent->setWindowTitle(_windowTitle);
		_changed = false;
		_changedIdentifier = false;
		_currentIdentifier = QString::fromStdString(npcFileList[index].first);
		_currentFile = file;
		_npcFileList = npcFileList;

		// load new NPC
		rpg::npc::NPCManager::GetSingleton().createNPC(identifierLineEdit->text().toStdString(), Vec3::ZERO, Quaternion::IDENTITY, false);
	}

	void NPCEditWidget::saveToFile(const std::string & file) {
		if (identifierLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setText(QApplication::tr("No identifier set!"));
			box.exec();
			return;
		}
		if (nameLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setText(QApplication::tr("No Name set!"));
			box.exec();
			return;
		}
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLDeclaration * decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		doc.InsertFirstChild(decl);

		tinyxml2::XMLElement * root = doc.NewElement("NPC");
		root->SetAttribute("identifier", identifierLineEdit->text().toStdString().c_str());
		doc.InsertAfterChild(decl, root);

		tinyxml2::XMLElement * name = doc.NewElement("Name");
		tinyxml2::XMLText * nameValue = doc.NewText(nameLineEdit->text().toStdString().c_str());
		name->LinkEndChild(nameValue);
		root->LinkEndChild(name);

		tinyxml2::XMLElement * currentXP = doc.NewElement("XP");
		tinyxml2::XMLText * currentXPValue = doc.NewText(std::to_string(currentXPSpinBox->value()).c_str());
		currentXP->LinkEndChild(currentXPValue);
		root->LinkEndChild(currentXP);

		tinyxml2::XMLElement * nextXP = doc.NewElement("NextXP");
		tinyxml2::XMLText * nextXPValue = doc.NewText(std::to_string(nextXPSpinBox->value()).c_str());
		nextXP->LinkEndChild(nextXPValue);
		root->LinkEndChild(nextXP);

		tinyxml2::XMLElement * level = doc.NewElement("Level");
		tinyxml2::XMLText * levelValue = doc.NewText(std::to_string(levelSpinBox->value()).c_str());
		level->LinkEndChild(levelValue);
		root->LinkEndChild(level);

		tinyxml2::XMLElement * model = doc.NewElement("Model");
		tinyxml2::XMLText * modelValue = doc.NewText(modelLineEdit->text().toStdString().c_str());
		model->LinkEndChild(modelValue);
		root->LinkEndChild(model);

		tinyxml2::XMLElement * currentHP = doc.NewElement("HP");
		tinyxml2::XMLText * currentHPValue = doc.NewText(std::to_string(currentHPSpinBox->value()).c_str());
		currentHP->LinkEndChild(currentHPValue);
		root->LinkEndChild(currentHP);

		tinyxml2::XMLElement * maxHP = doc.NewElement("MaxHP");
		tinyxml2::XMLText * maxHPValue = doc.NewText(std::to_string(maxHPSpinBox->value()).c_str());
		maxHP->LinkEndChild(maxHPValue);
		root->LinkEndChild(maxHP);

		clockUtils::iniParser::IniParser iniParser;
		if (clockUtils::ClockError::SUCCESS != iniParser.load("RPG.ini")) {
			QMessageBox box;
			box.setText(QApplication::tr("RPG.ini not found!"));
			box.exec();
		}
		std::string NPCDirectory;
		if (iniParser.getValue("SCRIPT", "npcDirectory", NPCDirectory) != clockUtils::ClockError::SUCCESS) {
			QMessageBox box;
			box.setText(QApplication::tr("No entry for npcDirectory in RPG.ini!"));
			box.exec();
		}
		doc.SaveFile(file.c_str());

		if (_changedIdentifier) {
			_npcListWidget->refreshNPCList();
		}

		_changed = false;
		_changedIdentifier = false;

		_parent->setWindowTitle(_windowTitle);
	}

} /* namespace widgets */
} /* namespace npcCreator */
} /* namespace tools */
} /* namespace i6e */
