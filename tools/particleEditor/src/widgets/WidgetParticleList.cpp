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

#include "widgets/WidgetParticleList.h"

#include "ParticleUniverseSystemManager.h"

#include <QFileDialog>
#include <QMessageBox>

namespace i6e {
namespace particleEditor {
namespace widgets {

	WidgetParticleList::WidgetParticleList(QWidget * par) : QWidget(par), _currentParticleTemplate(), _templateMap(), _system(nullptr), _dirty(false), _script(), _systemFileMapping() {
		setupUi(this);

		refreshParticleList();

		connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(selectParticle(QTreeWidgetItem *)));
	}

	WidgetParticleList::~WidgetParticleList() {
	}

	void WidgetParticleList::selectParticle(QTreeWidgetItem * item) {
		if (_currentParticleTemplate != item->text(0)) {
			if (_dirty) {
				if (QMessageBox::question(this, QApplication::tr("Unsaved changes"), QApplication::tr("There are unsaved changes for particle ") + _currentParticleTemplate + ".\n " + QApplication::tr("Do you want to save?"), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No) == QMessageBox::StandardButton::Yes) {
					saveParticle();
				}
			}
			_currentParticleTemplate = item->text(0);
			_dirty = false;
			emit createNewSystem(item->text(0));
		}
	}

	void WidgetParticleList::selectParticle(QString templateName) {
		selectParticle(_templateMap[templateName]);
	}

	void WidgetParticleList::setNewParticleSystem(ParticleUniverse::ParticleSystem * system) {
		_system = system;
		_script = QString::fromStdString(ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system));
		_dirty = false;
	}

	void WidgetParticleList::notifyChanged() {
		_dirty = true;
	}

	void WidgetParticleList::saveParticle() {
		if (_dirty) {
			auto it = _systemFileMapping.find(_currentParticleTemplate);
			QString file;
			if (it != _systemFileMapping.end()) {
				file = it->second;
			} else {
				file = QFileDialog::getSaveFileName(nullptr, QApplication::tr("Save file ..."), QString::fromStdString("../media/particles"), QApplication::tr("Particle Files (*.pu)"));
			}
			if (!file.isEmpty()) {
				ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_system, file.toStdString());
				_systemFileMapping[_currentParticleTemplate] = file;
			}
			_dirty = false;
		}
	}

	void WidgetParticleList::refreshParticleList() {
		_currentParticleTemplate = "";
		_templateMap.clear();
		ParticleUniverse::vector<ParticleUniverse::String> names;
		ParticleUniverse::ParticleSystemManager::getSingletonPtr()->particleSystemTemplateNames(names);
		treeWidget->clear();
		for (ParticleUniverse::String s : names) {
			QTreeWidgetItem * twi = new QTreeWidgetItem(treeWidget, { QString::fromStdString(s) });
			_templateMap.insert(std::make_pair(QString::fromStdString(s), twi));
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
