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

#ifndef __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__
#define __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__

#include "ui_widgetRender.h"

#include <chrono>

#include "i6engine/math/i6eVector.h"
#include "i6engine/math/i6eVector2.h"

namespace Ogre {
	class Camera;
	class ManualObject;
	class Overlay;
	class OverlayContainer;
	class RenderWindow;
	class Root;
	class SceneManager;
	class SceneNode;
	class TextAreaOverlayElement;
	class Viewport;
} /* namespace Ogre */

namespace ParticleUniverse {
	class ParticleSystem;
} /* namespace ParticleUniverse */

namespace i6e {
namespace modules {
	class ResourceManager;
} /* namespace modules */
namespace particleEditor {
namespace widgets {

	class WidgetRender : public QWidget, public Ui::renderWidget {
		Q_OBJECT

	public:
		/**
		 * \brief constructor
		 */
		WidgetRender(QWidget * par);

		/**
		 * \brief destructor
		 */
		~WidgetRender();

		ParticleUniverse::ParticleSystem * getParticleSystem() const {
			return _currentParticleSystemForRenderer;
		}

		void changeDimensions(const Vec3 & dimensions);

		Vec3 getDimensions() const {
			return _dimensions;
		}

	public slots:
		void play();
		void pause();
		void stop();
		void createNewSystem(const QString & particle);

	signals:
		void triggerRender();
		void setNewParticleSystem(ParticleUniverse::ParticleSystem *);
		void loadScript(ParticleUniverse::ParticleSystem *);

	private slots:
		void render();

	private:
		Ogre::Root * _root;
		modules::ResourceManager * _resourceManager;
		Ogre::RenderWindow * _rWindow;
		Ogre::SceneManager * _sceneManager;
		Ogre::Overlay * _overlay;
		Ogre::OverlayContainer * _textPanel;
		Ogre::TextAreaOverlayElement * _averageFPS;
		Ogre::SceneNode * _cameraNode;
		Ogre::Camera * _camera;
		Ogre::Viewport * _viewport;
		Ogre::SceneNode * _particleNode;
		Ogre::ManualObject * _box;
		ParticleUniverse::ParticleSystem * _currentParticleSystemForRenderer;
		size_t _maxNumberOfVisualParticles;
		size_t _maxNumberOfEmittedParticles;
		std::chrono::time_point<std::chrono::high_resolution_clock> _lastFrameTime;
		uint64_t _frames;
		Vec2i _lastPos;
		Vec3 _dimensions;

		void updateOverlay();
		void resizeEvent(QResizeEvent * evt) override;
		void wheelEvent(QWheelEvent * evt) override;
		void mouseMoveEvent(QMouseEvent * evt) override;
		void mousePressEvent(QMouseEvent * evt) override;
		void mouseReleaseEvent(QMouseEvent * evt) override;
		void zoom(double zoomFactor);
		void showEvent(QShowEvent * evt) override;

		void drawBox();
	};

} /* namespace widgets */
} /* namespace particleEditor */
} /* namespace i6e */

#endif /* __I6ENGINE_PARTICLEEDITOR_WIDGETS_WIDGETRENDER_H__ */
