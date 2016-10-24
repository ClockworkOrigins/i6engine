/*
 * SocketPerformanceTester
 * Copyright (2016) Daniel Bonrath, Michael Baer, All rights reserved.
 *
 * This file is part of SocketPerformanceTester; SocketPerformanceTester is free software; you can redistribute it and/or
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

#include "widgets/AboutDialog.h"

#include <iostream>

#include "i6engine/i6engineBuildSettings.h"

#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

namespace i6e {
namespace tools {
namespace common {

	AboutDialog::AboutDialog(QWidget * parent) : QDialog(parent, Qt::Popup) {
		QGridLayout * gridLayout = new QGridLayout(this);
		setWindowIcon(QIcon(":/icon.png"));
		QVBoxLayout * layout = new QVBoxLayout();
		QFont f;
		f.setBold(true);
		setFont(f);
		layout->addWidget(new QLabel(QString("i6engine ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH), this));
		layout->addWidget(new QLabel(QString("Clockwork Origins"), this));
		QLabel * cwMail = new QLabel(QString("Contact: <a href=\"mailto:contact@clockwork-origins.de\">contact@clockwork-origins.de</a>"), this);
		cwMail->setOpenExternalLinks(true);
		layout->addWidget(cwMail);
		QLabel * cwLink = new QLabel(QString("Homepage: <a href=\"http://clockwork-origins.de/\">http://clockwork-origins.de/</a>"), this);
		cwLink->setOpenExternalLinks(true);
		layout->addWidget(cwLink);
		QLabel * githubLink = new QLabel(QString("GitHub: <a href=\"https://github.com/ClockworkOrigins/i6engine\">https://github.com/ClockworkOrigins/i6engine</a>"), this);
		githubLink->setOpenExternalLinks(true);
		layout->addWidget(githubLink);
		layout->addWidget(new QLabel(QString(""), this));
		layout->addWidget(new QLabel(QString("Build for Qt ") + QT_VERSION_STR, this));
		layout->addWidget(new QLabel(QString("Running with Qt ") + qVersion(), this));
		QDir dir(QApplication::applicationDirPath() + "/../copyright/");
		layout->addWidget(new QLabel(QString("License: " + dir.absolutePath() + "/Qt LICENSE"), this));
		QLabel * qtLink = new QLabel(QString("Homepage: <a href=\"https://www.qt.io/\">https://www.qt.io/</a>"), this);
		qtLink->setOpenExternalLinks(true);
		layout->addWidget(qtLink);
		QPixmap pixmap(":/clockworkLogo.png");
		pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation);
		QLabel * logoLabel = new QLabel(this);
		logoLabel->setPixmap(pixmap);
		gridLayout->addWidget(logoLabel, 0, 0);
		gridLayout->addLayout(layout, 0, 1);
		setLayout(gridLayout);
	}

	AboutDialog::~AboutDialog() {
	}

} /* namespace common */
} /* namespace tools */
} /* namespace i6e */
