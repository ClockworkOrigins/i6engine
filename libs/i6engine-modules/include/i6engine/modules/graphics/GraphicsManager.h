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

/**
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_GRAPHICSMANAGER_H__
#define __I6ENGINE_MODULES_GRAPHICSMANAGER_H__

#include "i6engine/utils/i6eThreadSafety.h"

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/GameMessage.h"

#include "OGRE/OgreFrameListener.h"
#include "OGRE/OgreWindowEventUtilities.h"

namespace Ogre {
	class LogManager;
	class OverlaySystem;
	class RaySceneQuery;
	class RenderWindow;
	class Root;
	class SceneManager;
} /* namespace Ogre */

namespace i6e {
namespace modules {

	class Debug;
	class GraphicsNode;
	class GraphicsController;
	class GUIController;
	class ListenerFactoryLogic;
	class ResourceManager;
	class Terrain;

	class ISIXE_MODULES_API GraphicsManager : public Ogre::WindowEventListener, public Ogre::FrameListener {
	public:
		/**
		 * \brief initializes Ogre graphics
		 */
		GraphicsManager(GraphicsController * ctrl, HWND hWnd);

		/**
		 * \brief deletes all Ogre stuff
		 */
		~GraphicsManager();

		/**
		 * \brief Returns a pointer the root element of the graphics library
		 * \return Root Element of the graphics library
		 */
		inline Ogre::Root * getRoot() const { return _objRoot; }

		/**
		 * \brief Returns a pointer to the sceneManager of the object
		 *
		 * Returns a pointer to the sceneManager of the object
		 *
		 * \return SceneManager of the object
		 */
		inline Ogre::SceneManager * getSceneManager() const { return _sceneManager; }

		/**
		 * \brief Gets or creates a graphic node for given goid
		 */
		GraphicsNode * getOrCreateGraphicsNode(const int64_t goid, const Vec3 & position = Vec3(0.0, 0.0, 0.0), const Quaternion & rotation = Quaternion(1.0, 0.0, 0.0, 0.0), const Vec3 & scale = Vec3(1.0, 1.0, 1.0));

		/**
		 * \brief Gets a graphic node for given goid
		 *
		 * \param[in] goid ID for the graphic node
		 */
		GraphicsNode * getGraphicsNode(const int64_t goid) const;

		/**
		 * \brief Deletes GraphicsNode by goid
		 *
		 *     Deletes GraphicsNode by goid
		 */
		void deleteGraphicsNode(const int64_t goid);

		/**
		 * \brief addTerrain
		 *
		 *     Writes terrain object to an array by index (possibly overwriting!)
		 */
		void addTerrain(const int64_t id, const boost::shared_ptr<Terrain> & t);

		/**
		 * \brief removeTerrain
		 *
		 *     Removes terrain object in array by index.
		 */
		void removeTerrain(const int64_t id);

		/**
		 * \brief Called on every tick of the graphics subsystem
		 *
		 *     In effect the main render-loop
		 *     invokes Ogre's _objRoot->renderOneFrame()
		 *			calls tick on GUIController and injectTimePulse on CEGUI
		 *
		 *			Updates the GUI and tells Ogre to render the scene.
		 */
		void Tick();

		/**
		 * \brief windowClosing
		 *
		 *     Tells core to shutdown
		 *     (Thereby possibly adding a shutdown message to the message pipe which in turn will call GraphicsController::ShutdownImpl() back in this module?)
		 *
		 * \param   Ogre::RenderWindow * rw
		 * \return   bool: true on success
		 */
		bool windowClosing(Ogre::RenderWindow * rw) override;

		/**
		 * \brief handles all create messages for the graphics subsystem
		 */
		void NewsCreate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles all update messages for the graphics subsystem
		 */
		void NewsUpdate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles all delete messages for the graphics subsystem
		 */
		void NewsDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles all create messages for the graphic nodes
		 */
		void NewsNodeCreate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles all update messages for the graphic nodes
		 */
		void NewsNodeUpdate(const api::GameMessage::Ptr & msg);

		/**
		 * \brief handles all delete messages for the graphic nodes
		 */
		void NewsNodeDelete(const api::GameMessage::Ptr & msg);

		/**
		 * \brief adds GraphicsNode to tickList
		 */
		void addTicker(GraphicsNode * gn);

		/**
		 * \brief removes GraphicsNode to tickList
		 */
		void removeTicker(GraphicsNode * gn);

		Ogre::RenderWindow * getRenderWindow() const {
			return _rWindow;
		}

	private:
		/**
		 * Render window
		 */
		Ogre::RenderWindow * _rWindow;

		/**
		 * Root element of the graphics library
		 */
		Ogre::Root * _objRoot;

		/**
		 * SceneManager of the graphics library
		 */
		Ogre::SceneManager * _sceneManager;

		/**
		 * GraphicNodes of the graphics library
		 */
		std::map<int64_t, GraphicsNode *> _nodes;

		/**
		 * \brief
		 */
		std::map<int64_t, boost::shared_ptr<Terrain>> _terrains;

		/**
		 * \brief
		 */
		ResourceManager * _resourceManager;

		/**
		 * \brief
		 */
		Debug * _debug;

		Ogre::RaySceneQuery * _raySceneQuery;

		/**
		 * \brief list of all tickable GraphicNodes
		 */
		std::set<GraphicsNode *> _tickers;

		/**
		 * \brief
		 */
		GUIController * _guiController;

		GraphicsController * _ctrl;

		bool _initialized;

		bool _showFPS;

		Ogre::OverlaySystem * _overlaySystem;

		Ogre::LogManager * _logManager;

		std::vector<ListenerFactoryLogic *> _compositorLogics;

		Ogre::Camera * _mainCamera;
		Ogre::Viewport * _mainViewport;

		void loadResources(const std::string & resourcesFile, const std::function<void(uint16_t)> & callback);

		/**
		 * \brief called after rendering a frame
		 * actually used to get current frame time
		 */
		bool frameEnded(const Ogre::FrameEvent & evt) override;

		void windowResized(Ogre::RenderWindow * rw) override;
		void windowClosed(Ogre::RenderWindow * rw) override;

		/**
		 * \brief forbidden
		 */
		GraphicsManager(const GraphicsManager &) = delete;

		/**
		 * \brief forbidden
		 */
		GraphicsManager & operator=(const GraphicsManager &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GRAPHICSMANAGER_H__ */

/**
 * @}
 */
