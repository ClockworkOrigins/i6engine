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

#include "widgets/MainWindow.h"

#include "i6engine/i6engineBuildSettings.h"

#include "widgets/ConfigDialog.h"
#include "widgets/PropertyWindow.h"
#include "widgets/WidgetEdit.h"
#include "widgets/WidgetParticleList.h"
#include "widgets/WidgetRender.h"
#include "widgets/WidgetScript.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"
#include "ParticleUniverseTechnique.h"

#include <QToolBar>

namespace i6e {
namespace particleEditor {
namespace widgets {

	MainWindow::MainWindow(QMainWindow * par) : QMainWindow(par), _renderWrapper(new QWidget(this)), _renderWidget(new WidgetRender(this)), _particleListWidget(new WidgetParticleList(this)), _editWidget(new WidgetEdit(this, _renderWidget)), _scriptWidget(new WidgetScript(this)), _tabWidget(new QTabWidget(this)), _toolBarEdit(nullptr), _playing(false), _toolbarActions(), _currentTab(CurrentTab::Render), _particleSystemCounter(0), _leftLayout(nullptr), _currentPropertyWindow(nullptr) {
		setupUi(this);

		setWindowIcon(QIcon(":/icon.png"));

		showMaximized();

		setWindowTitle(QString("ParticleEditor (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QWidget * toolBarWrapper = new QWidget(this);
		QHBoxLayout * toolBarLayout = new QHBoxLayout(toolBarWrapper);
		toolBarWrapper->setLayout(toolBarLayout);
		QToolBar * tb = new QToolBar(toolBarWrapper);
		QAction * newAction = tb->addAction(QIcon(":/images/new.png"), QApplication::tr("New Particle"));
		QAction * cloneAction = tb->addAction(QIcon(":/images/clone.png"), QApplication::tr("Clone Particle"));
		QAction * saveAction = tb->addAction(QIcon(":/images/save.png"), QApplication::tr("Save Particle"));
		tb->addSeparator();
		QAction * playAction = tb->addAction(QIcon(":/images/control_play.png"), QApplication::tr("Play"));
		QAction * pauseAction = tb->addAction(QIcon(":/images/control_pause.png"), QApplication::tr("Pause"));
		QAction * stopAction = tb->addAction(QIcon(":/images/control_stop.png"), QApplication::tr("Stop"));

		connect(newAction, SIGNAL(triggered()), this, SLOT(handleNewAction()));
		connect(cloneAction, SIGNAL(triggered()), this, SLOT(handleCloneAction()));
		connect(saveAction, SIGNAL(triggered()), _particleListWidget, SLOT(saveParticle()));

		connect(playAction, SIGNAL(triggered()), this, SLOT(handlePlayAction()));
		connect(this, SIGNAL(triggerPlay()), _renderWidget, SLOT(play()));
		connect(pauseAction, SIGNAL(triggered()), this, SLOT(handlePauseAction()));
		connect(this, SIGNAL(triggerPause()), _renderWidget, SLOT(pause()));
		connect(stopAction, SIGNAL(triggered()), this, SLOT(handleStopAction()));
		connect(this, SIGNAL(triggerStop()), _renderWidget, SLOT(stop()));

		connect(_particleListWidget, SIGNAL(createNewSystem(const QString &)), this, SLOT(createNewSystem(const QString &)));
		connect(this, SIGNAL(triggerCreateNewSystem(const QString &)), _renderWidget, SLOT(createNewSystem(const QString &)));
		connect(_renderWidget, SIGNAL(setNewParticleSystem(ParticleUniverse::ParticleSystem *)), _editWidget, SLOT(setNewParticleSystem(ParticleUniverse::ParticleSystem *)));
		connect(_renderWidget, SIGNAL(setNewParticleSystem(ParticleUniverse::ParticleSystem *)), _particleListWidget, SLOT(setNewParticleSystem(ParticleUniverse::ParticleSystem *)));
		connect(_renderWidget, SIGNAL(loadScript(ParticleUniverse::ParticleSystem *)), _scriptWidget, SLOT(loadScript(ParticleUniverse::ParticleSystem *)));
		connect(this, SIGNAL(updateScript(ParticleUniverse::ParticleSystem *)), _scriptWidget, SLOT(loadScript(ParticleUniverse::ParticleSystem *)));
		connect(_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
		
		gridLayout->addWidget(toolBarWrapper, 0, 0);

		QWidget * hWidget = new QWidget(this);
		QHBoxLayout * hLayout = new QHBoxLayout(hWidget);

		gridLayout->addWidget(hWidget, 1, 0);

		QWidget * leftWidget = new QWidget(this);
		_leftLayout = new QVBoxLayout(leftWidget);
		leftWidget->setLayout(_leftLayout);
		_leftLayout->addWidget(_particleListWidget);

		hLayout->addWidget(leftWidget);
		hLayout->addWidget(_tabWidget);

		QVBoxLayout * vLayout = new QVBoxLayout(_renderWrapper);

		_renderWrapper->setLayout(vLayout);
		vLayout->addWidget(_renderWidget);

		_tabWidget->addTab(_renderWrapper, QApplication::tr("Render"));
		_tabWidget->addTab(_editWidget, QApplication::tr("Edit"));
		_tabWidget->addTab(_scriptWidget, QApplication::tr("Script"));

		hLayout->setStretch(0, 1);
		hLayout->setStretch(1, 3);

		_toolbarActions.insert(std::make_pair("New", newAction));
		_toolbarActions.insert(std::make_pair("Clone", cloneAction));
		_toolbarActions.insert(std::make_pair("Play", playAction));
		_toolbarActions.insert(std::make_pair("Pause", pauseAction));
		_toolbarActions.insert(std::make_pair("Stop", stopAction));

		_toolBarEdit = new QToolBar(_editWidget);
		QAction * tbEditTechniqueAction = _toolBarEdit->addAction(QIcon(":/images/technique.png"), QApplication::tr("Add a new technique"));
		QAction * tbEditRendererAction = _toolBarEdit->addAction(QIcon(":/images/renderer.png"), QApplication::tr("Add a new renderer"));
		QAction * tbEditEmitterAction = _toolBarEdit->addAction(QIcon(":/images/emitter.png"), QApplication::tr("Add a new emitter"));
		QAction * tbEditAffectorAction = _toolBarEdit->addAction(QIcon(":/images/affector.png"), QApplication::tr("Add a new affector"));
		QAction * tbEditObserverAction = _toolBarEdit->addAction(QIcon(":/images/observer.png"), QApplication::tr("Add a new observer"));
		QAction * tbEditHandlerAction = _toolBarEdit->addAction(QIcon(":/images/handler.png"), QApplication::tr("Add a new handler"));
		QAction * tbEditBehaviourAction = _toolBarEdit->addAction(QIcon(":/images/behaviour.png"), QApplication::tr("Add a new behaviour"));
		QAction * tbEditExternAction = _toolBarEdit->addAction(QIcon(":/images/extern.png"), QApplication::tr("Add a new extern"));
		_toolBarEdit->addSeparator();
		QAction * tbEditConnectAction = _toolBarEdit->addAction(QIcon(":/images/connect.png"), QApplication::tr("Make connection between two components"));
		QAction * tbEditDisconnectAction = _toolBarEdit->addAction(QIcon(":/images/disconnect.png"), QApplication::tr("Delete a connection between two components"));
		QAction * tbEditCursorAction = _toolBarEdit->addAction(QIcon(":/images/cursor.png"), QApplication::tr("Reset to default cursor"));

		connect(tbEditAffectorAction, SIGNAL(triggered()), _editWidget, SLOT(addNewAffector()));
		connect(tbEditTechniqueAction, SIGNAL(triggered()), _editWidget, SLOT(addNewTechnique()));
		connect(tbEditRendererAction, SIGNAL(triggered()), _editWidget, SLOT(addNewRenderer()));
		connect(tbEditEmitterAction, SIGNAL(triggered()), _editWidget, SLOT(addNewEmitter()));
		connect(tbEditObserverAction, SIGNAL(triggered()), _editWidget, SLOT(addNewObserver()));
		connect(tbEditHandlerAction, SIGNAL(triggered()), _editWidget, SLOT(addNewHandler()));
		connect(tbEditBehaviourAction, SIGNAL(triggered()), _editWidget, SLOT(addNewBehaviour()));
		connect(tbEditExternAction, SIGNAL(triggered()), _editWidget, SLOT(addNewExtern()));
		connect(tbEditConnectAction, SIGNAL(triggered()), _editWidget, SLOT(addConnection()));
		connect(tbEditDisconnectAction, SIGNAL(triggered()), _editWidget, SLOT(removeConnection()));
		connect(tbEditCursorAction, SIGNAL(triggered()), _editWidget, SLOT(triggerResetConnectionMode()));

		toolBarLayout->addWidget(tb);
		toolBarLayout->addWidget(_toolBarEdit);

		_toolBarEdit->hide();

		connect(_editWidget, SIGNAL(setPropertyWindow(PropertyWindow *)), this, SLOT(setPropertyWindow(PropertyWindow *)));
		connect(_editWidget, SIGNAL(renameParticleSystem(QString, QString)), this, SLOT(renameParticleSystem(QString, QString)));
		connect(_editWidget, SIGNAL(notifyChanged()), _particleListWidget, SLOT(notifyChanged()));
		connect(_scriptWidget, SIGNAL(notifyChanged()), _particleListWidget, SLOT(notifyChanged()));

		menuFile->setTitle(QApplication::tr("File"));
		actionExit->setText(QApplication::tr("Exit"));
		actionExit->setIcon(QIcon(":/images/close.png"));

		menuExtras->setTitle(QApplication::tr("Extras"));
		actionOptions->setText(QApplication::tr("Options"));

		actionOptions->setIcon(QIcon(":/images/config_general.png"));
	}

	MainWindow::~MainWindow() {
	}

	void MainWindow::closeEditor() {
		qApp->exit();
	}

	void MainWindow::handleNewAction() {
		QString templateName;
		
		do {
			templateName = "ParticleSystem" + QString::number(_particleSystemCounter++);
		} while (_particleListWidget->existsTemplateName(templateName));

		// Create new particle system template. If the name is changed in the (script) editor, the template name is updated.
		ParticleUniverse::ParticleSystem * pSys = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystemTemplate(templateName.toStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Set a default technique with renderer and emitter
		if (pSys) {
			ParticleUniverse::ParticleTechnique * technique = pSys->createTechnique();
			if (technique) {
				technique->createEmitter("Point");
				technique->setRenderer("Billboard");
			}
		}

		_particleListWidget->refreshParticleList();
		_particleListWidget->selectParticle(templateName);
	}

	void MainWindow::handleCloneAction() {
		QString templateName = _particleListWidget->getTemplateName();
		if (!templateName.isEmpty()) {
			// It is no category
			templateName = "CopyOf" + templateName;
			QString backup = templateName;
			int i = 0;
			while (_particleListWidget->existsTemplateName(templateName)) {
				templateName = backup + QString::number(++i);
			}
			// Create new particle system template.
			ParticleUniverse::ParticleSystem * templateSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystemTemplate(templateName.toStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			*templateSystem = *_renderWidget->getParticleSystem();
			_renderWidget->getParticleSystem()->copyAttributesTo(templateSystem);

			_particleListWidget->refreshParticleList();
			_particleListWidget->selectParticle(templateName);
		}
	}

	void MainWindow::handlePlayAction() {
		if (_playing || _currentParticleTemplate.size() == 0) {
			return;
		}
		_playing = true;
		_toolbarActions["Play"]->setIcon(QIcon(":/images/control_play.png"));
		_toolbarActions["Pause"]->setIcon(QIcon(":/images/control_pause_blue.png"));
		_toolbarActions["Stop"]->setIcon(QIcon(":/images/control_stop_blue.png"));
		emit triggerPlay();
	}

	void MainWindow::handlePauseAction() {
		if (!_playing) {
			return;
		}
		_playing = false;
		_toolbarActions["Play"]->setIcon(QIcon(":/images/control_play_blue.png"));
		_toolbarActions["Pause"]->setIcon(QIcon(":/images/control_pause.png"));
		_toolbarActions["Stop"]->setIcon(QIcon(":/images/control_stop.png"));
		emit triggerPause();
	}

	void MainWindow::handleStopAction() {
		if (!_playing) {
			return;
		}
		_playing = false;
		_toolbarActions["Play"]->setIcon(QIcon(":/images/control_play_blue.png"));
		_toolbarActions["Pause"]->setIcon(QIcon(":/images/control_pause.png"));
		_toolbarActions["Stop"]->setIcon(QIcon(":/images/control_stop.png"));
		emit triggerStop();
	}

	void MainWindow::createNewSystem(const QString & particle) {
		if (_currentParticleTemplate.size() == 0) {
			_toolbarActions["Play"]->setIcon(QIcon(":/images/control_play_blue.png"));
		}
		_currentParticleTemplate = particle;
		emit triggerCreateNewSystem(particle);
		if (_playing) {
			emit triggerPlay();
		}
	}

	void MainWindow::tabChanged(int index) {
		if (_currentTab == CurrentTab::Script && CurrentTab(index) == CurrentTab::Render) {
			if (!_scriptWidget->getScript().isEmpty()) {
				if (ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_renderWidget->getParticleSystem()) != _scriptWidget->getScript().toStdString()) {
					// Script has changed. Reparse and update the templates
					Ogre::String oldTemplateName = _particleListWidget->getTemplateName().toStdString();
					ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
					particleSystemManager->destroyParticleSystemTemplate(oldTemplateName); // Delete the old template
					_scriptWidget->parseScript();

					// Replace the old particle system with a new one, of which the name can also be changed.
					QString newTemplateName = QString::fromStdString(particleSystemManager->getLastCreatedParticleSystemTemplateName());

					_particleListWidget->refreshParticleList();
					_particleListWidget->selectParticle(newTemplateName);
				}
			}
		} else if (_currentTab == CurrentTab::Edit && CurrentTab(index) == CurrentTab::Render) {
			_editWidget->verticalLayout->removeWidget(_renderWidget);
			_renderWrapper->layout()->addWidget(_renderWidget);
			_toolBarEdit->hide();
			if (_currentPropertyWindow) {
				_currentPropertyWindow->hide();
			}
		} else if (_currentTab == CurrentTab::Edit && CurrentTab(index) == CurrentTab::Script) {
			_editWidget->verticalLayout->removeWidget(_renderWidget);
			_renderWrapper->layout()->addWidget(_renderWidget);
			_toolBarEdit->hide();
			if (_currentPropertyWindow) {
				_currentPropertyWindow->hide();
			}
			emit updateScript(_renderWidget->getParticleSystem());
		} else if (_currentTab == CurrentTab::Render && CurrentTab(index) == CurrentTab::Edit) {
			_renderWrapper->layout()->removeWidget(_renderWidget);
			_editWidget->verticalLayout->addWidget(_renderWidget);

			_editWidget->verticalLayout->setStretch(0, 1);
			_editWidget->verticalLayout->setStretch(1, 1);
			_toolBarEdit->show();
			if (_currentPropertyWindow) {
				_currentPropertyWindow->show();
			}
		} else if (_currentTab == CurrentTab::Script && CurrentTab(index) == CurrentTab::Edit) {
			_renderWrapper->layout()->removeWidget(_renderWidget);
			_editWidget->verticalLayout->addWidget(_renderWidget);

			_editWidget->verticalLayout->setStretch(0, 1);
			_editWidget->verticalLayout->setStretch(1, 1);
			_toolBarEdit->show();
			if (_currentPropertyWindow) {
				_currentPropertyWindow->show();
			}

			if (!_scriptWidget->getScript().isEmpty()) {
				if (ParticleUniverse::ParticleSystemManager::getSingleton().writeScript(_renderWidget->getParticleSystem()) != _scriptWidget->getScript().toStdString()) {
					// Script has changed. Reparse and update the templates
					Ogre::String oldTemplateName = _particleListWidget->getTemplateName().toStdString();
					ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
					particleSystemManager->destroyParticleSystemTemplate(oldTemplateName); // Delete the old template
					_scriptWidget->parseScript();

					// Replace the old particle system with a new one, of which the name can also be changed.
					QString newTemplateName = QString::fromStdString(particleSystemManager->getLastCreatedParticleSystemTemplateName());

					_particleListWidget->refreshParticleList();
					_particleListWidget->selectParticle(newTemplateName);
				}
			}
		}

		_currentTab = CurrentTab(index);
	}

	void MainWindow::setPropertyWindow(PropertyWindow * propertyWindow) {
		if (_currentTab != CurrentTab::Edit) {
			if (_currentPropertyWindow) {
				_leftLayout->removeWidget(_currentPropertyWindow);
				_currentPropertyWindow->hide();
				_currentPropertyWindow = nullptr;
			}
			return;
		}
		if (_currentPropertyWindow) {
			_leftLayout->removeWidget(_currentPropertyWindow);
			_currentPropertyWindow->hide();
		}
		if (propertyWindow) {
			_leftLayout->addWidget(propertyWindow);
			propertyWindow->show();
			_leftLayout->setStretch(0, 1);
			_leftLayout->setStretch(1, 1);
		}
		_currentPropertyWindow = propertyWindow;
	}

	void MainWindow::renameParticleSystem(QString oldName, QString newName) {
		// Replace the old particle system with a new one, of which the name can also be changed.
		ParticleUniverse::ParticleSystemManager * particleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
		QString newTemplateName = QString::fromStdString(particleSystemManager->getLastCreatedParticleSystemTemplateName());
		particleSystemManager->destroyParticleSystemTemplate(oldName.toStdString()); // Delete the old template

		// Script has changed. Reparse and update the templates
		_particleListWidget->refreshParticleList();
		_particleListWidget->selectParticle(newTemplateName);
	}

	void MainWindow::openOptions() {
		ConfigDialog dlg;
		dlg.boxXValue->setValue(_renderWidget->getDimensions().getX());
		dlg.boxYValue->setValue(_renderWidget->getDimensions().getY());
		dlg.boxZValue->setValue(_renderWidget->getDimensions().getZ());
		if (dlg.exec() == QDialog::Accepted) {
			Vec3 dimensions(dlg.boxXValue->value(), dlg.boxYValue->value(), dlg.boxZValue->value());
			_renderWidget->changeDimensions(dimensions);
		}
	}

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */
