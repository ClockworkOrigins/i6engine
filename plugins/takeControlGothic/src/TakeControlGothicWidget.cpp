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

#include "TakeControlGothicWidget.h"

#include "clockUtils/iniParser/iniParser.h"

#include <QApplication>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace i6e {
namespace plugins {

	TakeControlGothicWidget::TakeControlGothicWidget(clockUtils::iniParser::IniParser * iniParser, QWidget * par) : SettingsWidget(par), _iniParser(iniParser), _lineEdit(nullptr) {
		QHBoxLayout * l = new QHBoxLayout();

		std::string path;
		if (clockUtils::ClockError::SUCCESS == _iniParser->getValue("GOTHIC", "scriptPath", path)) {
		}
		QString qPath = QString::fromStdString(path);
		if (qPath.isEmpty()) {
			qPath = qApp->applicationDirPath();
		}

		QLabel * gothicPathLabel = new QLabel(QApplication::tr("GothicSourcePath"), this);
		_lineEdit = new QLineEdit(qPath, this);
		_lineEdit->setReadOnly(true);
		_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Ignored);
		QPushButton * gothicPathPushButton = new QPushButton("...", this);
		l->addWidget(gothicPathLabel);
		l->addWidget(_lineEdit);
		l->addWidget(gothicPathPushButton);
		connect(gothicPathPushButton, SIGNAL(clicked()), this, SLOT(openFileDialog()));

		setLayout(l);
		l->setAlignment(Qt::AlignTop);

		setMinimumWidth(600);
		setMinimumHeight(50);
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	}

	QString TakeControlGothicWidget::getGothicPath() const {
		return _lineEdit->text();
	}

	void TakeControlGothicWidget::saveSettings() {
		_iniParser->setValue("GOTHIC", "scriptPath", _lineEdit->text().toStdString());
		emit gothicPathChanged(_lineEdit->text());
	}

	void TakeControlGothicWidget::rejectChanges() {
		std::string path;
		if (clockUtils::ClockError::SUCCESS == _iniParser->getValue("GOTHIC", "scriptPath", path)) {
			_lineEdit->setText(QString::fromStdString(path));
		}
	}

	void TakeControlGothicWidget::openFileDialog() {
		QString path = QFileDialog::getExistingDirectory(this, QApplication::tr("SelectGothicDir"), _lineEdit->text());
		if (!path.isEmpty()) {
			if (_lineEdit->text() != path) {
				_lineEdit->setText(path);
			}
		}
	}

} /* namespace plugins */
} /* namespace i6e */
