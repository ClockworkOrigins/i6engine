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

#ifndef __I6ENGINE_TOOLS_MAINWINDOW_H__
#define __I6ENGINE_TOOLS_MAINWINDOW_H__

#include <chrono>

#include "i6engine/math/i6eQuaternion.h"

#include "OGRE/OgreWindowEventUtilities.h"

#include <QMainWindow>

class QSlider;

namespace Ogre {
	class AnimationState;
	class Camera;
	class LogManager;
	class RenderWindow;
	class Root;
	class SceneManager;
	class SceneNode;
} /* namespace Ogre */

namespace i6e {
namespace modules {
	class ResourceManager;
} /* namespace modules */
namespace tools {

	class MainWindow : public QMainWindow, public Ogre::WindowEventListener {
		Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

	signals:
		void triggerRender();

	private slots:
		void render();
		void selectModel();
		void selectAnimation();
		void takeScreenshot();
		void scaledModel(int value);
		void positionedModel(int value);

	private:
		QWidget * _centralWidget;
		Ogre::Root * _root;
		Ogre::RenderWindow * _renderWindow;
		Ogre::SceneManager * _sceneManager;
		Ogre::LogManager * _logManager;
		i6e::modules::ResourceManager * _resourceManager;
		Ogre::SceneNode * _cameraNode;
		Ogre::Camera * _camera;
		Ogre::SceneNode * _meshNode;
		Ogre::Entity * _mesh;
		Ogre::AnimationState * _animationState;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastFrameTime;
		QWidget * _renderWidget;
		QSlider * _scaleSlider;
		QSlider * _posSlider;
		bool _leftMouseButtonPressed;
		bool _rightMouseButtonPressed;
		int _lastX;
		int _lastY;
		Quaternion _lastRotX;
		Quaternion _lastRotY;

		void resizeEvent(QResizeEvent * evt) override;
		void mouseMoveEvent(QMouseEvent * evt) override;
		void mousePressEvent(QMouseEvent * evt) override;
		void mouseReleaseEvent(QMouseEvent * evt) override;
		void wheelEvent(QWheelEvent * evt) override;

		void zoom(double zoomFactor);
	};

} /* namespace tools */
} /* namespace i6e */

#endif /* __I6ENGINE_TOOLS_MAINWINDOW_H__ */
