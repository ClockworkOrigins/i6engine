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

#include "CollisionFlagsDialog.h"

#include <QApplication>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFile>
#include <QGroupBox>
#include <QInputDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QTextStream>
#include <QVBoxLayout>

namespace i6e {
namespace plugins {

	CollisionFlagsDialog::CollisionFlagsDialog(QWidget * par) : QDialog(par), _collisionGroup(), _collisionTypes(), _staticTypeCheckbox(nullptr), _ghostTypeCheckbox(nullptr), _triggerTypeCheckbox(nullptr), _ownCollisionTypesLayout(nullptr), _ownCollisionTypes(), _collidableCollisionTypesLayout(nullptr), _collidableCollisionTypes() {
		loadCollisionTypes();
		createGUI();
	}

	void CollisionFlagsDialog::setCollisionGroup(const api::CollisionGroup & cg) {
		_collisionGroup = cg;
		resetCollisionGroup();
	}

	api::CollisionGroup CollisionFlagsDialog::getCollisionGroup() const {
		return _collisionGroup;
	}

	void CollisionFlagsDialog::setReadOnly(bool readOnly) {
		_staticTypeCheckbox->setEnabled(!readOnly);
		_ghostTypeCheckbox->setEnabled(!readOnly);
		_triggerTypeCheckbox->setEnabled(!readOnly);
		for (int i = 0; i < _ownCollisionTypes.size(); i++) {
			_ownCollisionTypes[i]->setEnabled(!readOnly);
		}
		_collisionGroup.crashMask = 0;
		for (int i = 0; i < _collidableCollisionTypes.size(); i++) {
			_collidableCollisionTypes[i]->setEnabled(!readOnly);
		}
	}

	void CollisionFlagsDialog::updateCollisionGroup() {
		int responseType = api::ResponseType::ResponseType::NONE;
		if (_staticTypeCheckbox->isChecked()) {
			responseType |= int(api::ResponseType::ResponseType::STATIC);
		}
		if (_ghostTypeCheckbox->isChecked()) {
			responseType |= int(api::ResponseType::ResponseType::GHOST);
		}
		if (_triggerTypeCheckbox->isChecked()) {
			responseType |= int(api::ResponseType::ResponseType::TRIGGER);
		}
		_collisionGroup.responseType = responseType;
		for (int i = 0; i < _ownCollisionTypes.size(); i++) {
			if (_ownCollisionTypes[i]->isChecked()) {
				_collisionGroup.crashType = (1 << i);
				break;
			}
		}
		_collisionGroup.crashMask = 0;
		for (int i = 0; i < _collidableCollisionTypes.size(); i++) {
			if (_collidableCollisionTypes[i]->isChecked()) {
				_collisionGroup.crashMask |= (1 << i);
			}
		}
		saveCollisionTypes();
	}

	void CollisionFlagsDialog::resetCollisionGroup() {
		_staticTypeCheckbox->setChecked(_collisionGroup.responseType & api::ResponseType::ResponseType::STATIC);
		_ghostTypeCheckbox->setChecked(_collisionGroup.responseType & api::ResponseType::ResponseType::GHOST);
		_triggerTypeCheckbox->setChecked(_collisionGroup.responseType & api::ResponseType::ResponseType::TRIGGER);
		for (int i = 0; i < _ownCollisionTypes.size(); i++) {
			_ownCollisionTypes[i]->setChecked(_collisionGroup.crashType == (1 << i));
		}
		for (int i = 0; i < _collidableCollisionTypes.size(); i++) {
			_collidableCollisionTypes[i]->setChecked(_collisionGroup.crashMask & (1 << i));
		}
	}

	void CollisionFlagsDialog::addCollisionType() {
		QInputDialog dlg(this);
		dlg.setInputMode(QInputDialog::InputMode::TextInput);
		if (QDialog::Accepted == dlg.exec()) {
			if (!dlg.textValue().isEmpty()) {
				QRadioButton * rb = new QRadioButton(dlg.textValue(), _ownCollisionTypesLayout->parentWidget());
				_ownCollisionTypesLayout->addWidget(rb, _ownCollisionTypes.size() / 5, _ownCollisionTypes.size() % 5);
				_ownCollisionTypes.push_back(rb);

				QCheckBox * cb = new QCheckBox(dlg.textValue(), _collidableCollisionTypesLayout->parentWidget());
				_collidableCollisionTypesLayout->addWidget(cb, _collidableCollisionTypes.size() / 5, _collidableCollisionTypes.size() % 5);
				_collidableCollisionTypes.push_back(cb);

				_collisionTypes.push_back(dlg.textValue());
			}
		}
	}

	void CollisionFlagsDialog::loadCollisionTypes() {
		_collisionTypes.clear();
		QFile f(qApp->applicationDirPath() + "/../media/collisionTypes");
		if (f.open(QIODevice::ReadOnly)) {
			QTextStream ts(&f);
			while (!ts.atEnd()) {
				QString line = ts.readLine();
				if (!line.isEmpty()) {
					_collisionTypes.push_back(line);
				}
			}
		}
	}

	void CollisionFlagsDialog::saveCollisionTypes() {
		QFile f(qApp->applicationDirPath() + "/../media/collisionTypes");
		if (f.open(QIODevice::WriteOnly)) {
			QTextStream ts(&f);
			for (QString s : _collisionTypes) {
				ts << s << "\n";
			}
		}
	}

	void CollisionFlagsDialog::createGUI() {
		QVBoxLayout * l = new QVBoxLayout();
		QPushButton * pb = new QPushButton("+", this);
		pb->setToolTip(QApplication::tr("AddCollisionTypeTooltip"));
		pb->setStatusTip(QApplication::tr("AddCollisionTypeTooltip"));
		connect(pb, SIGNAL(clicked()), this, SLOT(addCollisionType()));
		l->addWidget(pb);
		{
			QGroupBox * responseTypeBox = new QGroupBox(QApplication::tr("ResponseTypes"), this);
			QHBoxLayout * responseTypeLayout = new QHBoxLayout();
			_staticTypeCheckbox = new QCheckBox(QApplication::tr("StaticResponseType"), responseTypeBox);
			_staticTypeCheckbox->setToolTip(QApplication::tr("StaticReponseTypeTooltip"));
			_staticTypeCheckbox->setStatusTip(QApplication::tr("StaticReponseTypeTooltip"));
			responseTypeLayout->addWidget(_staticTypeCheckbox);
			_ghostTypeCheckbox = new QCheckBox(QApplication::tr("GhostResponseType"), responseTypeBox);
			_ghostTypeCheckbox->setToolTip(QApplication::tr("GhostReponseTypeTooltip"));
			_ghostTypeCheckbox->setStatusTip(QApplication::tr("GhostReponseTypeTooltip"));
			responseTypeLayout->addWidget(_ghostTypeCheckbox);
			_triggerTypeCheckbox = new QCheckBox(QApplication::tr("TriggerResponseType"), responseTypeBox);
			_triggerTypeCheckbox->setToolTip(QApplication::tr("TriggerReponseTypeTooltip"));
			_triggerTypeCheckbox->setStatusTip(QApplication::tr("TriggerReponseTypeTooltip"));
			responseTypeLayout->addWidget(_triggerTypeCheckbox);
			responseTypeBox->setLayout(responseTypeLayout);
			responseTypeLayout->addStretch();
			l->addWidget(responseTypeBox);
		}
		{
			QGroupBox * ownCollisionTypeBox = new QGroupBox(QApplication::tr("OwnCollisionType"), this);
			ownCollisionTypeBox->setToolTip(QApplication::tr("OwnCollisionTypeTooltip"));
			ownCollisionTypeBox->setStatusTip(QApplication::tr("OwnCollisionTypeTooltip"));
			_ownCollisionTypesLayout = new QGridLayout();
			for (QString s : _collisionTypes) {
				QRadioButton * rb = new QRadioButton(s, ownCollisionTypeBox);
				_ownCollisionTypesLayout->addWidget(rb, _ownCollisionTypes.size() / 5, _ownCollisionTypes.size() % 5);
				_ownCollisionTypes.push_back(rb);
			}
			ownCollisionTypeBox->setLayout(_ownCollisionTypesLayout);
			l->addWidget(ownCollisionTypeBox);
		}
		{
			QGroupBox * collidableCollisionTypeBox = new QGroupBox(QApplication::tr("CollidableCollisionTypes"), this);
			collidableCollisionTypeBox->setToolTip(QApplication::tr("CollidableCollisionTypeTooltip"));
			collidableCollisionTypeBox->setStatusTip(QApplication::tr("CollidableCollisionTypeTooltip"));
			_collidableCollisionTypesLayout = new QGridLayout();
			for (QString s : _collisionTypes) {
				QCheckBox * cb = new QCheckBox(s, collidableCollisionTypeBox);
				_collidableCollisionTypesLayout->addWidget(cb, _collidableCollisionTypes.size() / 5, _collidableCollisionTypes.size() % 5);
				_collidableCollisionTypes.push_back(cb);
			}
			collidableCollisionTypeBox->setLayout(_collidableCollisionTypesLayout);
			l->addWidget(collidableCollisionTypeBox);
		}
		QDialogButtonBox * dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
		connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(updateCollisionGroup()));
		connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(resetCollisionGroup()));
		connect(dialogButtonBox, SIGNAL(accepted()), this, SLOT(hide()));
		connect(dialogButtonBox, SIGNAL(rejected()), this, SLOT(hide()));
		l->addWidget(dialogButtonBox);
		setLayout(l);
		l->addStretch();
	}

} /* namespace plugins */
} /* namespace i6e */
