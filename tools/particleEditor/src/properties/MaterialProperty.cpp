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

#include "properties/MaterialProperty.h"

#include "OGRE/OgreMaterialManager.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>

namespace i6e {
namespace particleEditor {
namespace properties {

	MaterialProperty::MaterialProperty(QWidget * par, QString label, QString name, QString value) : Property(par, label, name), _value(value), _lineEdit(nullptr) {
		_lineEdit = new QLineEdit(_value, this);
		horizontalLayout->addWidget(_lineEdit);
		QPushButton * materialButton = new QPushButton("...", this);
		horizontalLayout->addWidget(materialButton);
		connect(_lineEdit, SIGNAL(editingFinished()), this, SLOT(stringChanged()));
		connect(materialButton, SIGNAL(clicked()), this, SLOT(selectMaterial()));
	}

	MaterialProperty::~MaterialProperty() {
	}

	void MaterialProperty::setString(QString value) {
		_value = value;
		_lineEdit->setText(value);
	}

	void MaterialProperty::stringChanged() {
		if (_value != _lineEdit->text()) {
			_value = _lineEdit->text();
			triggerChangedSignal();
		}
	}

	void MaterialProperty::selectMaterial() {
		QStringList materialList;
		Ogre::ResourceManager::ResourceMapIterator materialIterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
		while (materialIterator.hasMoreElements()) {
			materialList.append(QString::fromStdString(materialIterator.peekNextValue()->getName()));
			materialIterator.moveNext();
		}
		QInputDialog dlg(this);
		dlg.setComboBoxEditable(false);
		dlg.setComboBoxItems(materialList);
		dlg.setWindowTitle(QApplication::tr("Select Material"));
		dlg.setLabelText(QApplication::tr("Select a material:"));
		dlg.setInputMode(QInputDialog::InputMode::TextInput);
		if (QDialog::Accepted == dlg.exec()) {
			_lineEdit->setText(dlg.textValue());
		}
	}

} /* namespace properties */
} /* namespace particleEditor */
} /* namespace i6e */
