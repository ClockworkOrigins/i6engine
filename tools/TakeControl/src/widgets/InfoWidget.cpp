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

#include "widgets/InfoWidget.h"

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace i6e {
namespace takeControl {
namespace widgets {

	InfoWidget::InfoWidget(QWidget * par) : QWidget(par), _npcLabel(new QLabel(this)), _npcDescriptionLabel(new QLabel(this)), _sentenceLabel(new QLabel(this)), _dialogDescriptionLabel(new QLabel(this)), _spokenCheckBox(new QCheckBox(this)), _loadSoundFileButton(new QPushButton("Load File", this)) {
		QGridLayout * l = new QGridLayout();
		l->addWidget(_npcLabel, 0, 0);
		l->addWidget(_npcDescriptionLabel, 1, 0);
		l->addWidget(_sentenceLabel, 0, 1);
		l->addWidget(_dialogDescriptionLabel, 1, 1);
		l->addWidget(_spokenCheckBox, 0, 2);
		l->addWidget(_loadSoundFileButton, 1, 2);
		setLayout(l);

		_npcLabel->setText("NPC1");
		_npcDescriptionLabel->setText("This character is old and has a deep voice");
		_sentenceLabel->setText("This is sentence 1");
		_dialogDescriptionLabel->setText("Some dialog description");
		_spokenCheckBox->setText("Take spoken");
	}

	InfoWidget::~InfoWidget() {
	}

} /* namespace widgets */
} /* namespace takeControl */
} /* namespace i6e */
