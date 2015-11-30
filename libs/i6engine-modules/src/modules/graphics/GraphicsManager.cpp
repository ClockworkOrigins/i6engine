/**
 * Copyright 2012 FAU (Friedrich Alexander University of Erlangen-Nuremberg)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "i6engine/modules/graphics/GraphicsManager.h"

#include "i6engine/utils/Exceptions.h"
#include "i6engine/utils/i6eString.h"
#include "i6engine/utils/Logger.h"

#include "i6engine/math/i6eVector.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/Application.h"
#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/configs/GUIConfig.h"
#include "i6engine/api/configs/InputConfig.h"
#include "i6engine/api/facades/GraphicsFacade.h"
#include "i6engine/api/facades/GUIFacade.h"
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/graphics/Debug.h"
#include "i6engine/modules/graphics/ResourceManager.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/graphics/GraphicsNode.h"
#include "i6engine/modules/graphics/Terrain.h"
#include "i6engine/modules/graphics/compositors/GaussianBlurLogic.h"
#include "i6engine/modules/graphics/compositors/HDRLogic.h"
#include "i6engine/modules/graphics/compositors/HeatVisionLogic.h"
#include "i6engine/modules/gui/GUIController.h"
#include "i6engine/modules/gui/GUIMailbox.h"

#include "ParticleUniverseSystem.h"
#include "ParticleUniverseSystemManager.h"

#include "boost/lexical_cast.hpp"

#include "CEGUI/CEGUI.h"

#include "OGRE/OgreCompositionPass.h"
#include "OGRE/OgreCompositionTargetPass.h"
#include "OGRE/OgreCompositorManager.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/OgreMeshManager.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"

#include "tinyxml2.h"

namespace i6engine {
namespace modules {

	GraphicsManager::GraphicsManager(GraphicsController * ctrl) : _rWindow(), _objRoot(), _sceneManager(), _nodes(), _terrains(), _resourceManager(), _debug(), _raySceneQuery(), _tickers(), _guiController(new GUIController()), _ctrl(ctrl), _initialized(false), _showFPS(false) {
		ASSERT_THREAD_SAFETY_CONSTRUCTOR

		try {
			std::string ogrePath;
			if (clockUtils::ClockError::SUCCESS != api::EngineController::GetSingletonPtr()->getIniParser().getValue<std::string>("GRAPHIC", "ogreConfigsPath", ogrePath)) {
				ISIXE_LOG_ERROR("Graphics", "An exception has occurred: value ogreConfigsPath in section GRAPHIC not found!");
				api::EngineController::GetSingletonPtr()->stop();
				return;
			}
			Ogre::LogManager * lm = new Ogre::LogManager();
			lm->createLog("ogre.log", true, false, false);
			_objRoot = new Ogre::Root(ogrePath + "/plugins.cfg", ogrePath + "/ogre.cfg", "");

			// make sure, Resourcemanager is initialized
			_resourceManager = new ResourceManager(ogrePath);

			_objRoot->restoreConfig();

			_rWindow = _objRoot->initialise(true, api::EngineController::GetSingletonPtr()->getAppl()->getName());
		} catch (Ogre::Exception & e) {
			ISIXE_LOG_ERROR("Graphics", "An exception has occurred: " << e.what());
			std::cout << "An exception has occurred: " << e.what() << std::endl;
			api::EngineController::GetSingletonPtr()->stop();
			return;
		}

		_initialized = true;

		Ogre::WindowEventUtilities::addWindowEventListener(_rWindow, this);

		_sceneManager = _objRoot->createSceneManager(Ogre::ST_GENERIC);

		_sceneManager->addRenderQueueListener(new Ogre::OverlaySystem());

		_debug = new Debug(_sceneManager, 0.5f);

		_raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(Ogre::MIP_UNLIMITED);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		Ogre::CompositorManager & compMgr = Ogre::CompositorManager::getSingleton();
		compMgr.registerCompositorLogic("HDR", new HDRLogic());
		compMgr.registerCompositorLogic("GaussianBlur", new GaussianBlurLogic());
		compMgr.registerCompositorLogic("HeatVision", new HeatVisionLogic());

		Ogre::ConfigOptionMap & CurrentRendererOptions = _objRoot->getRenderSystem()->getConfigOptions();
		Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
		Ogre::StringVector res;
		while (configItr != CurrentRendererOptions.end()) {
			if ((configItr)->first == "Video Mode") {
				// Store Available Resolutions
				res = ((configItr)->second.possibleValues);
			}
			configItr++;
		}

		std::vector<api::graphics::Resolution> res2;
		for (auto s : res) {
			api::graphics::Resolution r;
			std::vector<std::string> tmp = utils::split(s, " ");
			r.width = boost::lexical_cast<uint32_t>(tmp.front());
			r.height = boost::lexical_cast<uint32_t>(tmp.back());
			res2.push_back(r);
		}

		api::EngineController::GetSingleton().getGraphicsFacade()->setPossibleResolutions(res2);

		// initalize MotionBlur
		Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create("MotionBlur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		{
			Ogre::CompositionTechnique * t = comp3->createTechnique();
			{
				Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			{
				Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("sum");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			{
				Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("temp");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			// Render scene
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				tp->setOutputName("scene");
			}
			// Initialisation pass for sum texture
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				tp->setOutputName("sum");
				tp->setOnlyInitial(true);
			}
			// Do the motion blur
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				tp->setOutputName("temp");
				{
					Ogre::CompositionPass * pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/Combine");
					pass->setInput(0, "scene");
					pass->setInput(1, "sum");
				}
			}
			// Copy back sum texture
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				tp->setOutputName("sum");
				{
					Ogre::CompositionPass * pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/Copyback");
					pass->setInput(0, "temp");
				}
			}
			// Display result
			{
				Ogre::CompositionTargetPass * tp = t->getOutputTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				{
					Ogre::CompositionPass * pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/MotionBlur");
					pass->setInput(0, "sum");
				}
			}
		}
		// Heat vision effect
		Ogre::CompositorPtr comp4 = Ogre::CompositorManager::getSingleton().create("HeatVision", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		{
			Ogre::CompositionTechnique * t = comp4->createTechnique();
			t->setCompositorLogicName("HeatVision");
			{
				Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
				def->width = 256;
				def->height = 256;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			{
				Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("temp");
				def->width = 256;
				def->height = 256;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			// Render scene
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				tp->setOutputName("scene");
			}
			// Light to heat pass
			{
				Ogre::CompositionTargetPass * tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				tp->setOutputName("temp");
				{
					Ogre::CompositionPass * pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setIdentifier(0xDEADBABE); /// Identify pass for use in listener
					pass->setMaterialName("Fury/HeatVision/LightToHeat");
					pass->setInput(0, "scene");
				}
			}
			// Display result
			{
				Ogre::CompositionTargetPass * tp = t->getOutputTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				{
					Ogre::CompositionPass *pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Fury/HeatVision/Blur");
					pass->setInput(0, "temp");
				}
			}
		}

		// Start the gui subsytem. Initialization is completely delegated to the GUIController.
		// Attention: The gui subsystem MUST be started after the renderer because it needs a valid window handle!
		_guiController->OnThreadStart();

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::input::InputWindow, core::Method::Create, new api::input::Input_Window_Create(reinterpret_cast<void *>(_objRoot)), core::Subsystem::Graphic);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		api::GameMessage::Ptr msg2 = boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, api::gui::GuiWindow, core::Method::Create, new api::gui::GUI_Window_Create(reinterpret_cast<void *>(_objRoot)), core::Subsystem::Graphic);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg2);

		_rWindow->resetStatistics();

#if ISIXE_MPLATFORM == ISIXE_MPLATFORM_WIN32
		SetCursor(nullptr);
		ShowCursor(false);
#endif
	}

	GraphicsManager::~GraphicsManager() {
		ASSERT_THREAD_SAFETY_FUNCTION

		_sceneManager->destroyQuery(_raySceneQuery);

		delete _debug;
		_debug = nullptr;

		_terrains.clear();

		if (_initialized) {
			_guiController->ShutDown();
		}

		for (std::pair<int64_t, GraphicsNode *> p : _nodes) {
			delete p.second;
		}

		delete _resourceManager;
		if (_objRoot != nullptr) {
			_objRoot->getAutoCreatedWindow()->destroy();
			_objRoot->shutdown();
			delete _objRoot;
		}
		delete _guiController;
	}

	void GraphicsManager::Tick() {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (!_initialized) {
			return;
		}

		if (_sceneManager && _rWindow->getNumViewports() > 0 && _rWindow->getViewport(0) && CEGUI::System::getSingletonPtr()) {
			// Setup the ray scene query
			CEGUI::Vector2f mousePos = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
			Ogre::Ray mouseRay = _rWindow->getViewport(0)->getCamera()->getCameraToViewportRay(mousePos.d_x / double(_rWindow->getWidth()), mousePos.d_y / double(_rWindow->getHeight()));
			_raySceneQuery->setRay(mouseRay);
			_raySceneQuery->setSortByDistance(true);
			// Execute query
			Ogre::RaySceneQueryResult & result = _raySceneQuery->execute();
			Ogre::RaySceneQueryResult::iterator itr;

			std::vector<std::pair<int64_t, Vec3>> results;

			// Get results
			for (itr = result.begin(); itr != result.end(); itr++) {
				//std::cout << "Object: " << itr->movable->getParentSceneNode()->getName() << std::endl;
				auto split = utils::split(itr->movable->getParentSceneNode()->getName(), "_");
				if (split.front() == "unnamed" || split.front() == "debug") {
					if (itr->worldFragment) {
						results.push_back(std::make_pair(-1, Vec3(itr->worldFragment->singleIntersection)));
					} else {
						results.push_back(std::make_pair(-1, Vec3(mouseRay.getPoint(itr->distance))));
					}
				} else {
					results.push_back(std::make_pair(std::stoi(split[1]), Vec3(mouseRay.getPoint(itr->distance))));
				}
			}
			api::EngineController::GetSingleton().getGraphicsFacade()->setSelectables(results);
		} else {
			api::EngineController::GetSingleton().getGraphicsFacade()->setSelectables(std::vector<std::pair<int64_t, Vec3>>());
		}

		for (auto gn : _tickers) {
			gn->Tick();
		}

		if (_showFPS) {
			Ogre::RenderTarget::FrameStats stats = _rWindow->getStatistics();
			api::GUIFacade * gf = api::EngineController::GetSingleton().getGUIFacade();
			std::ostringstream oss;
			oss.str("");
			oss << std::fixed << std::setprecision(1) << stats.lastFPS;
			gf->setText("FPS_Cur_Value", oss.str());
			oss.str("");
			oss << std::fixed << std::setprecision(1) << stats.avgFPS;
			gf->setText("FPS_Avg_Value", oss.str());
			oss.str("");
			oss << std::fixed << std::setprecision(1) << stats.bestFPS;
			gf->setText("FPS_Best_Value", oss.str());
			oss.str("");
			oss << std::fixed << std::setprecision(1) << stats.worstFPS;
			gf->setText("FPS_Worst_Value", oss.str());
		}

		try {
			Debug::getSingleton().build();
			_objRoot->renderOneFrame();
			Debug::getSingleton().clear();

			Ogre::WindowEventUtilities::messagePump();

			_guiController->Tick();
		} catch (utils::exceptions::SubsystemException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	GraphicsNode * GraphicsManager::getOrCreateGraphicsNode(const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) {
		ASSERT_THREAD_SAFETY_FUNCTION
		auto it = _nodes.find(goid);
		if (it == _nodes.end()) {
			GraphicsNode * gn = new GraphicsNode(this, goid, position, rotation, scale);
			_nodes.insert(std::make_pair(goid, gn));
			return gn;
		}
		return it->second;
	}

	GraphicsNode * GraphicsManager::getGraphicsNode(const int64_t goid) const {
		ASSERT_THREAD_SAFETY_FUNCTION
		auto it = _nodes.find(goid);
		if (it == _nodes.end()) {
			return nullptr;
		}
		return it->second;
	}

	void GraphicsManager::deleteGraphicsNode(const int64_t goid) {
		ASSERT_THREAD_SAFETY_FUNCTION
		auto it = _nodes.find(goid);
		if (it == _nodes.end()) {
			return;
		}
		delete it->second;
		_nodes.erase(it);
	}

	bool GraphicsManager::windowClosing(Ogre::RenderWindow * rw) {
		ASSERT_THREAD_SAFETY_FUNCTION
		return true;
	}

	void GraphicsManager::addTerrain(const int64_t id, const boost::shared_ptr<Terrain> & t) {
		ASSERT_THREAD_SAFETY_FUNCTION

		assert(_terrains.count(id) == 0);

		_terrains[id] = t;
	}

	void GraphicsManager::removeTerrain(const int64_t id) {
		ASSERT_THREAD_SAFETY_FUNCTION

		_terrains.erase(_terrains.find(id));
	}

	void GraphicsManager::NewsCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (msg->getSubtype() == api::graphics::GraLine) {
			Vec3 from = static_cast<api::graphics::Graphics_Line_Create *>(msg->getContent())->from;
			Vec3 to = static_cast<api::graphics::Graphics_Line_Create *>(msg->getContent())->to;

			Debug::getSingleton().drawLine(from.toOgre(), to.toOgre(), Ogre::ColourValue::Red);
		} else if (msg->getSubtype() == api::graphics::GraTerrain) {
			std::string heightmap = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->heightmap;
			double size = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->size;
			double inputScale = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->inputScale;
			uint32_t vertices = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->vertices;
			std::vector<std::tuple<double, std::string, std::string, double, double>> layers = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->layers;
			int64_t minX = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minX;
			int64_t minY = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minY;
			int64_t maxX = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxX;
			int64_t maxY = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxY;

			addTerrain(msg->getContent()->getID(), boost::make_shared<Terrain>(this, heightmap, size, inputScale, vertices, layers, minX, minY, maxX, maxY));
		} else if (msg->getSubtype() == api::graphics::GraSkyBox) {
			api::graphics::Graphics_SkyBox_Create * c = static_cast<api::graphics::Graphics_SkyBox_Create *>(msg->getContent());

			_sceneManager->setSkyBox(true, c->material, c->distance, c->renderFirst);
		} else if (msg->getSubtype() == api::graphics::GraSkyDome) {
			api::graphics::Graphics_SkyDome_Create * c = static_cast<api::graphics::Graphics_SkyDome_Create *>(msg->getContent());

			_sceneManager->setSkyDome(true, c->material, c->curvature, c->tiles, c->distance, c->renderFirst);
		} else if (msg->getSubtype() == api::graphics::GraSkyPlane) {
			api::graphics::Graphics_SkyPlane_Create * c = static_cast<api::graphics::Graphics_SkyPlane_Create *>(msg->getContent());

			_sceneManager->setSkyPlane(true, Ogre::Plane(Ogre::Vector3(0.0, c->direction, 0.0), c->distance), c->material, c->size, c->tiles, c->renderFirst, c->curvature, c->xSegments, c->ySegments);
		} else if (msg->getSubtype() == api::graphics::GraScreenshot) {
			api::graphics::Graphics_Screenshot_Create * gsc = dynamic_cast<api::graphics::Graphics_Screenshot_Create *>(msg->getContent());
			try {
				_rWindow->writeContentsToTimestampedFile(gsc->prefix, gsc->suffix);
			} catch (Ogre::Exception & e) {
				std::cout << e.what() << std::endl;;
			}
		} else if (msg->getSubtype() == api::graphics::GraFPS) {
			_showFPS = true;
		} else if (msg->getSubtype() == api::graphics::GraLoadResources) {
			api::graphics::Graphics_LoadResources_Create * glrc = dynamic_cast<api::graphics::Graphics_LoadResources_Create *>(msg->getContent());
			loadResources(glrc->resourcesFile);
			api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(glrc->msg);
		} else if (msg->getSubtype() == api::graphics::GraSaveTerrainShape) {
			api::graphics::Graphics_SaveTerrainShape_Create * gstsc = dynamic_cast<api::graphics::Graphics_SaveTerrainShape_Create *>(msg->getContent());
			assert(_terrains.find(gstsc->getWaitID()) != _terrains.end());
			_terrains[gstsc->getWaitID()]->saveCollisionShape(gstsc->file);
			gstsc->callback();
		} else {
			ISIXE_THROW_MESSAGE("GraphicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void GraphicsManager::NewsUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (msg->getSubtype() == api::graphics::GraAmbLight) {
			_sceneManager->setAmbientLight(Ogre::ColourValue(float(static_cast<api::graphics::Graphics_AmbLight_Update *>(msg->getContent())->red), float(static_cast<api::graphics::Graphics_AmbLight_Update *>(msg->getContent())->green), float(static_cast<api::graphics::Graphics_AmbLight_Update *>(msg->getContent())->blue)));
		} else if (msg->getSubtype() == api::graphics::GraResolution) {
			api::graphics::Graphics_Resolution_Update * ru = dynamic_cast<api::graphics::Graphics_Resolution_Update *>(msg->getContent());
			_rWindow->setFullscreen(_rWindow->isFullScreen(), ru->resolution.width, ru->resolution.height);

			_guiController->_mailbox->News(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, api::gui::GuiResolution, core::Method::Update, new api::gui::GUI_Resolution_Update(ru->resolution), core::Subsystem::Graphic));
			api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::input::InputResolution, core::Method::Update, new api::input::Input_Resolution_Update(ru->resolution), core::Subsystem::Graphic));

			Ogre::ConfigOptionMap & CurrentRendererOptions = _objRoot->getRenderSystem()->getConfigOptions();
			Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
			Ogre::StringVector res;
			while (configItr != CurrentRendererOptions.end()) {
				if (configItr->first == "Video Mode") {
					// Store Available Resolutions
					configItr->second.currentValue = boost::lexical_cast<std::string>(ru->resolution.width) + " x " + boost::lexical_cast<std::string>(ru->resolution.height);
					break;
				}
				configItr++;
			}
			_objRoot->saveConfig();
		} else if (msg->getSubtype() == api::graphics::GraFullscreen) {
			api::graphics::Graphics_Fullscreen_Update * ru = dynamic_cast<api::graphics::Graphics_Fullscreen_Update *>(msg->getContent());

			Ogre::ConfigOptionMap & CurrentRendererOptions = _objRoot->getRenderSystem()->getConfigOptions();
			Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();
			Ogre::StringVector res;
			while (configItr != CurrentRendererOptions.end()) {
				if (configItr->first == "Full Screen") {
					// Store Available Resolutions
					if (ru->fullscreen) {
						configItr->second.currentValue = "Yes";
					} else {
						configItr->second.currentValue = "No";
					}
				}
				if (configItr->first == "Video Mode") {
					std::vector<std::string> resolution = utils::split(configItr->second.currentValue, " ");
					_rWindow->setFullscreen(ru->fullscreen, boost::lexical_cast<uint32_t>(resolution.front()), boost::lexical_cast<uint32_t>(resolution.back()));
				}
				configItr++;
			}
			_objRoot->saveConfig();
		} else if (msg->getSubtype() == api::graphics::GraTerrain) {
			std::string heightmap = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->heightmap;
			double size = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->size;
			double inputScale = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->inputScale;
			uint32_t vertices = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->vertices;
			std::vector<std::tuple<double, std::string, std::string, double, double>> layers = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->layers;
			int64_t minX = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minX;
			int64_t minY = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->minY;
			int64_t maxX = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxX;
			int64_t maxY = static_cast<api::graphics::Graphics_Terrain_Create *>(msg->getContent())->maxY;

			removeTerrain(msg->getContent()->getID());
			addTerrain(msg->getContent()->getID(), boost::make_shared<Terrain>(this, heightmap, size, inputScale, vertices, layers, minX, minY, maxX, maxY));
		} else if (msg->getSubtype() == api::graphics::GraShadowTechnique) {
			api::graphics::ShadowTechnique st = dynamic_cast<api::graphics::Graphics_ShadowTechnique_Update *>(msg->getContent())->shadowTechnique;

			switch (st) {
			case api::graphics::ShadowTechnique::None: {
				_sceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_NONE);
				break;
			}
			case api::graphics::ShadowTechnique::Stencil_Additive: {
				_sceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);
				break;
			}
			case api::graphics::ShadowTechnique::Stencil_Modulative: {
				_sceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
				break;
			}
			default: {
				break;
			}
			}
		} else if (msg->getSubtype() == api::graphics::GraSetLinearFog) {
			api::graphics::Graphics_SetLinearFog_Update * gslfu = dynamic_cast<api::graphics::Graphics_SetLinearFog_Update *>(msg->getContent());
			_sceneManager->setFog(Ogre::FogMode::FOG_LINEAR, Ogre::ColourValue(gslfu->colour.getX(), gslfu->colour.getY(), gslfu->colour.getZ()), 0.0, gslfu->start, gslfu->end);
		} else if (msg->getSubtype() == api::graphics::GraSetExponentialFog) {
			api::graphics::Graphics_SetExponentialFog_Update * gsefu = dynamic_cast<api::graphics::Graphics_SetExponentialFog_Update *>(msg->getContent());
			_sceneManager->setFog(Ogre::FogMode::FOG_EXP, Ogre::ColourValue(gsefu->colour.getX(), gsefu->colour.getY(), gsefu->colour.getZ()), gsefu->density);
		} else if (msg->getSubtype() == api::graphics::GraSetExponentialFog2) {
			api::graphics::Graphics_SetExponentialFog_Update * gsefu = dynamic_cast<api::graphics::Graphics_SetExponentialFog_Update *>(msg->getContent());
			_sceneManager->setFog(Ogre::FogMode::FOG_EXP2, Ogre::ColourValue(gsefu->colour.getX(), gsefu->colour.getY(), gsefu->colour.getZ()), gsefu->density);
		} else if (msg->getSubtype() == api::graphics::GraGetHighestCoordinate) {
			api::graphics::Graphics_GetHighestCoordinate_Update * ggu = dynamic_cast<api::graphics::Graphics_GetHighestCoordinate_Update *>(msg->getContent());
			Vec3 startPos = ggu->startPos;
			startPos.setY(DBL_MAX);
			Ogre::Ray ray(startPos.toOgre(), Ogre::Vector3(0.0, -1.0, 0.0));
			_raySceneQuery->setRay(ray);
			_raySceneQuery->setSortByDistance(true);
			// Execute query
			Ogre::RaySceneQueryResult & result = _raySceneQuery->execute();
			Ogre::RaySceneQueryResult::iterator itr;

			if (result.empty()) {
				ggu->callback(Vec3::ZERO);
			} else {
				ggu->callback(Vec3(ray.getPoint(result[0].distance)));
			}
		} else {
			ISIXE_THROW_MESSAGE("GraphicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void GraphicsManager::NewsDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (msg->getSubtype() == api::graphics::GraViewports) {
			_objRoot->getAutoCreatedWindow()->removeAllViewports();
		} else if (msg->getSubtype() == api::graphics::GraTerrain) {
			removeTerrain(static_cast<api::graphics::Graphics_Terrain_Delete *>(msg->getContent())->comp);
		} else if (msg->getSubtype() == api::graphics::GraReset) {
			_ctrl->reset();
		} else if (msg->getSubtype() == api::graphics::GraSkyBox) {
			_sceneManager->setSkyBox(false, "");
		} else if (msg->getSubtype() == api::graphics::GraSkyDome) {
			_sceneManager->setSkyDome(false, "");
		} else if (msg->getSubtype() == api::graphics::GraSkyPlane) {
			_sceneManager->setSkyPlane(false, Ogre::Plane(), "");
		} else if (msg->getSubtype() == api::graphics::GraFPS) {
			_showFPS = false;
		} else {
			ISIXE_THROW_MESSAGE("GraphicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void GraphicsManager::NewsNodeCreate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		int64_t goid = msg->getContent()->getWaitID();
		int64_t coid = msg->getContent()->getID();

		if (msg->getSubtype() == api::graphics::GraCamera) {
			api::graphics::Graphics_Camera_Create * m = static_cast<api::graphics::Graphics_Camera_Create *>(msg->getContent());
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createCameraComponent(coid, m->pos, m->look, m->nearClip, m->fov);
		} else if (msg->getSubtype() == api::graphics::GraLuminous) {
			uint16_t t = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->lightType;

			api::LuminousAppearanceComponent::LightType lightType = api::LuminousAppearanceComponent::LightType(t);

			Vec3 diffuse = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->diffuse;
			Vec3 specular = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->specular;
			Vec4 attenuation = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->attenuation;
			Vec3 direction = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->direction;
			double spotLightRangeInner = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeInner;
			double spotLightRangeOuter = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeOuter;
			Vec3 position = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->position;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createLuminousComponent(coid, lightType, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			std::string meshName = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->mesh;

			bool isVisible = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->visible;

			Vec3 p = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->pos;
			Quaternion r = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->rot;
			Vec3 s = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->scale;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->getOrCreateSceneNode(coid, p, r, s);
			node->createMeshComponent(coid, meshName, isVisible);
		} else if (msg->getSubtype() == api::graphics::GraViewport) {
			double left = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->left;
			double top = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->top;
			double width = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->width;
			double height = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->height;
			double red = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->red;
			double green = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->green;
			double blue = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->blue;
			double alpha = static_cast<api::graphics::Graphics_Viewport_Create *>(msg->getContent())->alpha;

			GraphicsNode * node = getGraphicsNode(goid);
			node->createOrUpdateViewport(coid, left, top, width, height, red, green, blue, alpha);
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			GraphicsNode * node = getOrCreateGraphicsNode(msg->getContent()->getID());

			Ogre::SceneNode * sceneNode = node->getSceneNode();

			Vec3 p = static_cast<api::graphics::Graphics_Node_Create *>(msg->getContent())->pos;

			sceneNode->setPosition(p.toOgre());
			// rotation
			Quaternion r = static_cast<api::graphics::Graphics_Node_Create *>(msg->getContent())->rot;

			sceneNode->setOrientation(r.toOgre());

			// Scale
			Vec3 s = static_cast<api::graphics::Graphics_Node_Create *>(msg->getContent())->scale;

			sceneNode->setScale(s.toOgre());

			api::EngineController::GetSingleton().getGraphicsFacade()->notifyNewID(static_cast<api::graphics::Graphics_Node_Create *>(msg->getContent())->coid);
		} else if (msg->getSubtype() == api::graphics::GraParticle) {
			api::graphics::Graphics_Particle_Create * g = static_cast<api::graphics::Graphics_Particle_Create *>(msg->getContent());

			std::string emitterName = g->emitterName;
			Vec3 pos = g->pos;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createParticleComponent(coid, emitterName, pos);
		} else if (msg->getSubtype() == api::graphics::GraBillboardSet) {
			api::graphics::Graphics_BillboardSet_Create * gbc = static_cast<api::graphics::Graphics_BillboardSet_Create *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createBilldboardSetComponent(coid, gbc->material, gbc->width, gbc->height, gbc->origin);
		} else if (msg->getSubtype() == api::graphics::GraMovableText) {
			api::graphics::Graphics_MovableText_Create * gmtc = static_cast<api::graphics::Graphics_MovableText_Create *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createMovableText(coid, gmtc->targetID, gmtc->font, gmtc->text, gmtc->size, gmtc->colour);
		} else {
			ISIXE_THROW_MESSAGE("GraphicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void GraphicsManager::NewsNodeUpdate(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION

		int64_t goid = msg->getContent()->getWaitID(); // FIXME: Achtung was goid und was wid ist :D
		int64_t coid = msg->getContent()->getID();

		if (msg->getSubtype() == api::graphics::GraCamera) { // TODO: (Michael) rewrite to switch?
			api::graphics::Graphics_Camera_Update * m = static_cast<api::graphics::Graphics_Camera_Update *>(msg->getContent());
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->updateCameraComponent(coid, m->pos, m->look, m->nearClip, m->fov);
		} else if (msg->getSubtype() == api::graphics::GraFrustum) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			api::graphics::Graphics_CameraFrustum_Update * m = static_cast<api::graphics::Graphics_CameraFrustum_Update *>(msg->getContent());
			node->updateCameraFrustumComponent(coid, m->left, m->right, m->top, m->bottom);
		} else if (msg->getSubtype() == api::graphics::GraLuminous) {
			int t = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->lightType;

			api::LuminousAppearanceComponent::LightType lightType = static_cast<api::LuminousAppearanceComponent::LightType>(t);

			Vec3 diffuse = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->diffuse; // TODO: (Michael) use a tmp variable for readability
			Vec3 specular = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->specular;
			Vec4 attenuation = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->attenuation;
			Vec3 direction = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->direction;
			double spotLightRangeInner = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeInner;
			double spotLightRangeOuter = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeOuter;
			Vec3 position = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->position;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->updateLuminousComponent(coid, lightType, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		} else if (msg->getSubtype() == api::graphics::GraMaterial) {
			std::string materialName = static_cast<api::graphics::Graphics_Material_Update *>(msg->getContent())->material;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);

			node->setMaterial(coid, materialName);
		} else if (msg->getSubtype() == api::graphics::GraCusParam) {
			api::graphics::Graphics_CusParam_Update * m = static_cast<api::graphics::Graphics_CusParam_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);

			node->setCustomParameter(m->num, m->value);
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			std::string meshName = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->mesh;

			bool isVisible = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->visible;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->updateMeshComponent(coid, meshName, isVisible);
		} else if (msg->getSubtype() == api::graphics::GraViewport) {
			double left = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->left;
			double top = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->top;
			double width = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->width;
			double height = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->height;
			double red = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->red;
			double green = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->green;
			double blue = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->blue;
			double alpha = static_cast<api::graphics::Graphics_Viewport_Update *>(msg->getContent())->alpha;

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createOrUpdateViewport(coid, left, top, width, height, red, green, blue, alpha);
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);

			Ogre::SceneNode * sceneNode = node->getSceneNode();

			Vec3 p = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->pos;

			sceneNode->setPosition(p.toOgre());
			// rotation
			Quaternion r = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->rot;

			sceneNode->setOrientation(r.toOgre());

			// Scale
			Vec3 s = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->scale;

			sceneNode->setScale(s.toOgre());
		} else if (msg->getSubtype() == api::graphics::GraPlayAnimation) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);

			api::graphics::Graphics_PlayAnimation_Update * gpu = dynamic_cast<api::graphics::Graphics_PlayAnimation_Update *>(msg->getContent());

			node->playAnimation(coid, gpu->anim, gpu->looping, gpu->offsetPercent);
		} else if (msg->getSubtype() == api::graphics::GraSetAnimationSpeed) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);

			api::graphics::Graphics_SetAnimationSpeed_Update * gsu = dynamic_cast<api::graphics::Graphics_SetAnimationSpeed_Update *>(msg->getContent());

			node->setAnimationSpeed(gsu->speed);
		} else if (msg->getSubtype() == api::graphics::GraStopAnimation) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->stopAnimation();
		} else if (msg->getSubtype() == api::graphics::GraBillboard) {
			api::graphics::Graphics_Billboard_Update * gbu = static_cast<api::graphics::Graphics_Billboard_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->createOrUpdateBillboard(coid, gbu->identifier, gbu->offset, gbu->width, gbu->height, gbu->u0, gbu->v0, gbu->u1, gbu->v1);
		} else if (msg->getSubtype() == api::graphics::GraBillboardRemove) {
			api::graphics::Graphics_BillboardRemove_Update * gbu = static_cast<api::graphics::Graphics_BillboardRemove_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteBillboard(coid, gbu->identifier);
		} else if (msg->getSubtype() == api::graphics::GraMovableText) {
			api::graphics::Graphics_MovableText_Update * gmtu = static_cast<api::graphics::Graphics_MovableText_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->updateMovableText(coid, gmtu->font, gmtu->text, gmtu->size, gmtu->colour);
		} else if (msg->getSubtype() == api::graphics::GraCompositor) {
			api::graphics::Graphics_Compositor_Update * gcu = dynamic_cast<api::graphics::Graphics_Compositor_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->enableCompositor(coid, gcu->compositor, gcu->enabled);
		} else if (msg->getSubtype() == api::graphics::GraParticleFadeOut) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->particleFadeOut(coid);
		} else if (msg->getSubtype() == api::graphics::GraDrawBB) {
			api::graphics::Graphics_DrawBB_Update * gdu = dynamic_cast<api::graphics::Graphics_DrawBB_Update *>(msg->getContent());
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->drawBoundingBox(coid, gdu->colour);
		} else if (msg->getSubtype() == api::graphics::GraRemoveBB) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->removeBoundingBox();
		} else {
			ISIXE_THROW_MESSAGE("GraphicsManager", "Unknown MessageSubType '" << msg->getSubtype() << "'");
		}
	}

	void GraphicsManager::NewsNodeDelete(const api::GameMessage::Ptr & msg) {
		ASSERT_THREAD_SAFETY_FUNCTION
		int64_t goid = msg->getContent()->getWaitID();
		int64_t coid = msg->getContent()->getID();

		if (msg->getSubtype() == api::graphics::GraCamera) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteCameraComponent(coid);
		} else if (msg->getSubtype() == api::graphics::GraLuminous) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteLuminousComponent(coid);
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteMeshComponent(coid);
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			std::cout << "Delete Node " << coid << std::endl;
			deleteGraphicsNode(coid);
		} else if (msg->getSubtype() == api::graphics::GraParticle) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteParticleComponent(coid);
		} else if (msg->getSubtype() == api::graphics::GraBillboardSet) {
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteBillboardSetComponent(coid);
		} else if (msg->getSubtype() == api::graphics::GraMovableText) {
			std::cout << "Delete Movable Text " << goid << std::endl;
			GraphicsNode * node = getGraphicsNode(goid);
			assert(node);
			node->deleteMovableText(coid);
		}
	}

	void GraphicsManager::addTicker(GraphicsNode * gn) {
		_tickers.insert(gn);
	}

	void GraphicsManager::removeTicker(GraphicsNode * gn) {
		_tickers.erase(gn);
	}

	void GraphicsManager::loadResources(const std::string & resourcesFile) {
		tinyxml2::XMLDocument doc;

		if (doc.LoadFile(resourcesFile.c_str())) {
			ISIXE_LOG_ERROR("GraphicsManager", "Couldn't open resources file (" << resourcesFile << ")");
			return;
		}

		tinyxml2::XMLElement * root = doc.FirstChildElement("Resources");

		std::vector<std::string> meshes;

		for (tinyxml2::XMLElement * mesh = root->FirstChildElement("Mesh"); mesh != nullptr; mesh = mesh->NextSiblingElement("Mesh")) {
			if (mesh->GetText() == nullptr) {
				ISIXE_LOG_ERROR("GraphicsManager", "Found Mesh entry without value!");
			}
			meshes.push_back(mesh->GetText());
		}

		std::vector<std::string> particles;

		for (tinyxml2::XMLElement * particle = root->FirstChildElement("Particle"); particle != nullptr; particle = particle->NextSiblingElement("Particle")) {
			if (particle->GetText() == nullptr) {
				ISIXE_LOG_ERROR("GraphicsManager", "Found Particle entry without value!");
			}
			particles.push_back(particle->GetText());
		}

		Ogre::SceneNode * sn = _sceneManager->getRootSceneNode()->createChildSceneNode("PreLoadSceneNode_0_0", Ogre::Vector3::ZERO);
		Ogre::Camera * camera = _sceneManager->createCamera("PreLoadSceneCamera_0_0");
		sn->attachObject(camera);
		_objRoot->getAutoCreatedWindow()->addViewport(camera, 0, 0.0, 0.0, 1.0, 1.0);

		for (std::string m : meshes) {
			Ogre::Entity * meshEntity = _sceneManager->createEntity("PreLoadSceneMesh_0_0", m);
			meshEntity->setVisible(true);
			meshEntity->setCastShadows(true);
			sn->attachObject(meshEntity);
			_objRoot->renderOneFrame();
			sn->detachObject(meshEntity);
			_sceneManager->destroyEntity(meshEntity);
		}

		for (std::string p : particles) {
			ParticleUniverse::ParticleSystem * particleSystem = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem("PreLoadSceneParticle_0_0", p, _sceneManager);
			sn->attachObject(particleSystem);
			particleSystem->start();
			_objRoot->renderOneFrame();
			sn->detachObject(particleSystem);
			particleSystem->stop();
			ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyParticleSystem(particleSystem, _sceneManager);
		}

		_objRoot->getAutoCreatedWindow()->removeViewport(0);
		sn->detachObject(camera);
		_sceneManager->destroyCamera(camera);
		_sceneManager->getRootSceneNode()->removeChild(sn);
		_sceneManager->destroySceneNode("PreLoadSceneNode_0_0");
	}

} /* namespace modules */
} /* namespace i6engine */
