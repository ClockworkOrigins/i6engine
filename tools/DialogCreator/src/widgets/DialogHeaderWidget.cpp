#include "widgets/DialogHeaderWidget.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

#include <QMessageBox>

#include "tinyxml2.h"

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	DialogHeaderWidget::DialogHeaderWidget(QWidget * par) : QWidget(par), _participants(), _dialogDirectory() {
		setupUi(this);

		_participants.push_back(new QLineEdit(this));
		participantsLayout->addWidget(_participants.back());
		connect(_participants.back(), SIGNAL(textChanged(const QString &)), this, SLOT(updatedParticipants()));
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
		numberSpinBox->setValue(dialog->nr);
		permanentCheckBox->setChecked(dialog->permanent);
		importantCheckBox->setChecked(dialog->important);
		descriptionLineEdit->setText(QString::fromStdString(dialog->description));
		conditionLineEdit->setText(QString::fromStdString(dialog->conditionScript));
		informationLineEdit->setText(QString::fromStdString(dialog->infoScript));
	}

	void DialogHeaderWidget::updatedParticipants() {
		for (size_t i = 0; i < _participants.size(); i++) {
			if (i < _participants.size() - 1) {
				if (_participants[i]->text().isEmpty()) {
					delete _participants[i];
					_participants.erase(_participants.begin() + i);
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
			box.setWindowTitle(QString("Incomplete data!"));
			box.setInformativeText("Identifier for dialog not set!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (_participants.size() == 1) {
			QMessageBox box;
			box.setWindowTitle(QString("Incomplete data!"));
			box.setInformativeText("A dialog has to have at least one participant!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (!importantCheckBox->isChecked() && descriptionLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setWindowTitle(QString("Incomplete data!"));
			box.setInformativeText("An unimportant dialog has to have a description!");
			box.setStandardButtons(QMessageBox::StandardButton::Ok);
			box.exec();
			return;
		}
		if (informationLineEdit->text().isEmpty()) {
			QMessageBox box;
			box.setWindowTitle(QString("Incomplete data!"));
			box.setInformativeText("Information script required!");
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

		emit refreshLists();
	}

	void DialogHeaderWidget::saveNewDialog() {
		saveChanges();
	}

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
