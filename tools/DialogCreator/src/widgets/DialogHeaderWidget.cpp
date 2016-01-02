#include "widgets/DialogHeaderWidget.h"

#include "i6engine/rpg/dialog/Dialog.h"
#include "i6engine/rpg/dialog/DialogManager.h"

namespace i6engine {
namespace dialogCreator {
namespace widgets {

	DialogHeaderWidget::DialogHeaderWidget(QWidget * par) : QWidget(par), _participants() {
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

} /* namespace widgets */
} /* namespace dialogCreator */
} /* namespace i6engine */
