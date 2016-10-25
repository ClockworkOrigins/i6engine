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

#include "MainWindow.h"

#include "i6engine/math/i6eMath.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/modules/graphics/ResourceManager.h"

#include "clockUtils/iniParser/iniParser.h"

#include "OGRE/OgreEntity.h"
#include "OGRE/OgreLogManager.h"
#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/OgreWindowEventUtilities.h"

#include <QAction>
#include <QApplication>
#include <QGridLayout>
#include <QInputDialog>
#include <QResizeEvent>
#include <QScreen>
#include <QSlider>
#include <QTime>
#include <QWindow>

namespace i6e {
namespace tools {

	const int FRAMES = 60;

	MainWindow::MainWindow() : QMainWindow(), _centralWidget(nullptr), _root(nullptr), _renderWindow(nullptr), _sceneManager(nullptr), _logManager(nullptr), _resourceManager(nullptr), _cameraNode(nullptr), _camera(nullptr), _meshNode(nullptr), _mesh(nullptr), _animationState(nullptr), _lastFrameTime(std::chrono::high_resolution_clock::now()), _renderWidget(nullptr), _scaleSlider(nullptr), _posSlider(nullptr), _leftMouseButtonPressed(false), _rightMouseButtonPressed(false), _lastX(), _lastY(), _lastRotX(Quaternion::IDENTITY), _lastRotY(Quaternion::IDENTITY) {
		setWindowTitle(QString("MeshPreview (v ") + QString::number(ISIXE_VERSION_MAJOR) + QString(".") + QString::number(ISIXE_VERSION_MINOR) + QString(".") + QString::number(ISIXE_VERSION_PATCH) + QString(")"));

		QGridLayout * layout = new QGridLayout();

		_centralWidget = new QWidget(this);
		setCentralWidget(_centralWidget);

		_renderWidget = new QWidget(_centralWidget);

		clockUtils::iniParser::IniParser iniParser;
		std::string iniPath = qApp->applicationDirPath().toStdString() + "/i6engine.ini";
		if (clockUtils::ClockError::SUCCESS != iniParser.load(qApp->applicationDirPath().toStdString() + "/i6engine.ini")) {
			return;
		}
		std::string ogrePath;
		if (clockUtils::ClockError::SUCCESS != iniParser.getValue("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			return;
		}
		_logManager = new Ogre::LogManager();
		_logManager->createLog("ogre.log", true, false, false);
		_root = new Ogre::Root(ogrePath + "/plugins.cfg", ogrePath + "/ogre.cfg", "");

		// make sure, Resourcemanager is initialized
		_resourceManager = new modules::ResourceManager(ogrePath);

		_root->restoreConfig();

		_root->initialise(false, "MeshPreview");
		Ogre::NameValuePairList misc;
		misc["externalWindowHandle"] = Ogre::StringConverter::toString((size_t) _renderWidget->winId());
		misc["parentWindowHandle"] = Ogre::StringConverter::toString((size_t) _renderWidget->winId());
		_renderWindow = _root->createRenderWindow("MeshPreview", size().width(), size().height(), false, &misc);

		Ogre::WindowEventUtilities::addWindowEventListener(_renderWindow, this);

		_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);
		_sceneManager->setAmbientLight(Ogre::ColourValue());

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(Ogre::MIP_UNLIMITED);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		_cameraNode = _sceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");
		_camera = _sceneManager->createCamera("MainCamera");
		_camera->setAutoAspectRatio(true);
		_camera->setPosition(Ogre::Vector3(0.0, 0.0, 5.0));
		_camera->setNearClipDistance(0.1);
		_camera->setFarClipDistance(99999.0);
		_cameraNode->attachObject(_camera);

		Ogre::Viewport * vp = _renderWindow->addViewport(_camera);
		vp->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f, 0.0f));

		layout->addWidget(_renderWidget, 0, 0);

		_scaleSlider = new QSlider(_centralWidget);
		_scaleSlider->setMaximum(1000);
		_scaleSlider->setValue(500);
		_scaleSlider->setMinimum(0);
		_scaleSlider->setOrientation(Qt::Orientation::Horizontal);

		connect(_scaleSlider, SIGNAL(valueChanged(int)), this, SLOT(scaledModel(int)), Qt::QueuedConnection);

		layout->addWidget(_scaleSlider, 1, 0);

		_posSlider = new QSlider(_centralWidget);
		_posSlider->setMaximum(1000);
		_posSlider->setValue(500);
		_posSlider->setMinimum(0);
		_posSlider->setOrientation(Qt::Orientation::Vertical);

		connect(_posSlider, SIGNAL(valueChanged(int)), this, SLOT(positionedModel(int)), Qt::QueuedConnection);

		layout->addWidget(_posSlider, 0, 1);

		_centralWidget->setLayout(layout);

		connect(this, SIGNAL(triggerRender()), this, SLOT(render()), Qt::QueuedConnection);

		QAction * selectModelAction = new QAction(this);
		selectModelAction->setShortcut(QKeySequence::StandardKey::Open);
		selectModelAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
		addAction(selectModelAction);
		connect(selectModelAction, SIGNAL(triggered()), this, SLOT(selectModel()), Qt::QueuedConnection);

		QAction * selectAnimationAction = new QAction(this);
		selectAnimationAction->setShortcut(QKeySequence::StandardKey::SelectAll);
		selectAnimationAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
		addAction(selectAnimationAction);
		connect(selectAnimationAction, SIGNAL(triggered()), this, SLOT(selectAnimation()), Qt::QueuedConnection);

		QAction * screenShotAction = new QAction(this);
		addAction(screenShotAction);
		screenShotAction->setShortcut(QKeySequence(Qt::Key_F12));
		screenShotAction->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);

		connect(screenShotAction, SIGNAL(triggered()), this, SLOT(takeScreenshot()));

		emit triggerRender();
	}

	MainWindow::~MainWindow() {
		delete _resourceManager;
		if (_root != nullptr) {
			_renderWindow->destroy();
			_root->shutdown();
			delete _root;
		}
		delete _logManager;
	}

	void MainWindow::render() {
		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
		if (uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(now - _lastFrameTime).count()) >= 1000000 / FRAMES) {
			_lastFrameTime = _lastFrameTime + std::chrono::microseconds(1000000 / FRAMES);
			if (_animationState) {
				_animationState->addTime(1.0 / FRAMES);
			}
			_root->renderOneFrame();
		}
		emit triggerRender();
	}

	void MainWindow::selectModel() {
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
		dlg.setWindowTitle(QApplication::tr("Select Mesh"));
		dlg.setLabelText(QApplication::tr("Select a Mesh:"));
		dlg.setInputMode(QInputDialog::InputMode::TextInput);
		if (QDialog::Accepted == dlg.exec()) {
			if (!_meshNode) {
				_meshNode = _sceneManager->getRootSceneNode()->createChildSceneNode("Mesh");
				_mesh = _sceneManager->createEntity(dlg.textValue().toStdString());
				_meshNode->attachObject(_mesh);
			} else {
				_meshNode->detachObject(_mesh);
				_sceneManager->destroyEntity(_mesh);
				_mesh = _sceneManager->createEntity(dlg.textValue().toStdString());
				_meshNode->attachObject(_mesh);
			}
			_animationState = nullptr;
			_scaleSlider->setValue(500);
			_meshNode->setScale(Ogre::Vector3(1.0, 1.0, 1.0));
			_posSlider->setValue(500);
			_meshNode->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
			_meshNode->setOrientation(Ogre::Quaternion::IDENTITY);
		}
	}

	void MainWindow::selectAnimation() {
		QStringList animationList;
		animationList.append("NONE");
		if (_meshNode && _mesh->getSkeleton()) {
			uint16_t numAnimations = _mesh->getSkeleton()->getNumAnimations();
			for (uint16_t i = 0; i < numAnimations; i++) {
				animationList.append(QString::fromStdString(_mesh->getSkeleton()->getAnimation(i)->getName()));
			}
		}
		if (animationList.size() > 1) {
			QInputDialog dlg(this);
			dlg.setComboBoxEditable(false);
			dlg.setComboBoxItems(animationList);
			dlg.setWindowTitle(QApplication::tr("Select Animation"));
			dlg.setLabelText(QApplication::tr("Select an Animation:"));
			dlg.setInputMode(QInputDialog::InputMode::TextInput);
			if (QDialog::Accepted == dlg.exec()) {
				if (_animationState) {
					_animationState->setEnabled(false);
				}
				if (dlg.textValue() == "NONE") {
					_animationState = nullptr;
				} else {
					_animationState = _mesh->getAnimationState(dlg.textValue().toStdString());
					_animationState->setEnabled(true);
					_animationState->setLoop(true);
				}
			}
		}
	}

	void MainWindow::takeScreenshot() {
		_renderWindow->writeContentsToTimestampedFile("Screen_", ".png");
	}

	void MainWindow::scaledModel(int value) {
		if (_meshNode) {
			_meshNode->setScale(Ogre::Vector3(1.0, 1.0, 1.0) * value / 500.0);
		}
	}

	void MainWindow::positionedModel(int value) {
		if (_meshNode) {
			_meshNode->setPosition(Ogre::Vector3(0.0, value * 0.1 - 50.0, 0.0));
		}
	}

	void MainWindow::resizeEvent(QResizeEvent * evt) {
		if (_renderWindow) {
			_renderWindow->resize(evt->size().width(), evt->size().height());
			_renderWindow->windowMovedOrResized();
		}
		evt->ignore();
	}

	void MainWindow::mouseMoveEvent(QMouseEvent * evt) {
		if (_leftMouseButtonPressed) {
			if (_meshNode) {
				_lastRotY = _lastRotY * Quaternion(Vec3(0.0, 1.0, 0.0), (evt->pos().x() - _lastX) * PI / 180.0);
				_meshNode->setOrientation(_lastRotY.toOgre());
			}
		} else if (_rightMouseButtonPressed) {
			if (_meshNode) {
				_lastRotX = _lastRotX * Quaternion(Vec3(1.0, 0.0, 0.0), (evt->pos().y() - _lastY) * PI / 180.0);
				_meshNode->setOrientation(_lastRotX.toOgre());
			}
		}
		_lastX = evt->pos().x();
		_lastY = evt->pos().y();
		evt->accept();
	}

	void MainWindow::mousePressEvent(QMouseEvent * evt) {
		if (evt->button() == Qt::MouseButton::LeftButton) {
			_leftMouseButtonPressed = true;
		} else if (evt->button() == Qt::MouseButton::RightButton) {
			_rightMouseButtonPressed = true;
		}
		_lastX = evt->pos().x();
		_lastY = evt->pos().y();
		evt->accept();
	}

	void MainWindow::mouseReleaseEvent(QMouseEvent * evt) {
		if (evt->button() == Qt::MouseButton::LeftButton) {
			_leftMouseButtonPressed = false;
		} else if (evt->button() == Qt::MouseButton::RightButton) {
			_rightMouseButtonPressed = false;
		}
		evt->accept();
	}

	void MainWindow::wheelEvent(QWheelEvent * evt) {
		if (evt->delta() > 0) {
			// Zoom in
			zoom(0.95);
		} else if (evt->delta() < 0) {
			// Zoom out
			zoom(1.05);
		}
		evt->accept();
	}

	void MainWindow::zoom(double zoomFactor) {
		Ogre::Vector3 cameraPosition = _camera->getPosition();
		Ogre::Vector3 pivot = Ogre::Vector3::ZERO;
		Ogre::Vector3 direction = cameraPosition - pivot;
		_camera->setPosition(pivot + zoomFactor * direction);
	}

} /* namespace tools */
} /* namespace i6e */
