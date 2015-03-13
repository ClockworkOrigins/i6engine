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
#include "i6engine/api/facades/MessagingFacade.h"

#include "i6engine/modules/graphics/Debug.h"
#include "i6engine/modules/graphics/ResourceManager.h"
#include "i6engine/modules/graphics/GraphicsController.h"
#include "i6engine/modules/graphics/GraphicsNode.h"
#include "i6engine/modules/graphics/Terrain.h"
#include "i6engine/modules/gui/GUIController.h"

#include "boost/lexical_cast.hpp"

#include "CEGUI/CEGUI.h"

#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreRoot.h"

namespace i6engine {
namespace modules {

	GraphicsManager::GraphicsManager(GraphicsController * ctrl) : _rWindow(), _objRoot(), _sceneManager(), _nodes(), _terrains(), _resourceManager(), _debug(), _raySceneQuery(), _tickers(), _guiController(new GUIController()), _ctrl(ctrl), _initialized(false) {
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

		_debug = new Debug(_sceneManager, 0.5f);

		_raySceneQuery = _sceneManager->createRayQuery(Ogre::Ray());

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(Ogre::MIP_UNLIMITED);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

		api::GameMessage::Ptr msg = boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::input::InputWindow, core::Method::Create, new api::input::Input_Window_Create(reinterpret_cast<void *>(_objRoot)), i6engine::core::Subsystem::Graphic);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);

		api::GameMessage::Ptr msg2 = boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, api::gui::GuiWindow, core::Method::Create, new api::gui::GUI_Window_Create(reinterpret_cast<void *>(_objRoot)), i6engine::core::Subsystem::Graphic);

		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg2);

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
			std::vector<std::string> tmp = i6engine::utils::split(s, " ");
			r.width = boost::lexical_cast<uint32_t>(tmp.front());
			r.height = boost::lexical_cast<uint32_t>(tmp.back());
			res2.push_back(r);
		}

		i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->setPossibleResolutions(res2);

		// Start the gui subsytem. Initialization is completely delegated to the GUIController.
		// Attention: The gui subsystem MUST be started after the renderer because it needs a valid window handle!
		_guiController->OnThreadStart();
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

		if (_sceneManager != nullptr && _rWindow->getNumViewports() > 0 && _rWindow->getViewport(0) != nullptr) {
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
					results.push_back(std::make_pair(std::atoi(split[1].c_str()), Vec3(mouseRay.getPoint(itr->distance))));
				}
			}
			i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->setSelectables(results);
		} else {
			i6engine::api::EngineController::GetSingleton().getGraphicsFacade()->setSelectables(std::vector<std::pair<int64_t, Vec3>>());
		}

		for (auto gn : _tickers) {
			gn->Tick();
		}

		try {
			Debug::getSingleton().build();
			_objRoot->renderOneFrame();
			Debug::getSingleton().clear();

			//std::cout << _rWindow->getAverageFPS() << std::endl;

			Ogre::WindowEventUtilities::messagePump();

			_guiController->Tick();
		} catch (utils::exceptions::SubsystemException & e) {
			e.writeLog();
			e.PassToMain();
		}
	}

	GraphicsNode * GraphicsManager::getOrCreateGraphicsNode(const int64_t goid, const Vec3 & position, const Quaternion & rotation, const Vec3 & scale) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_nodes.find(goid) == _nodes.end()) {
			_nodes[goid] = new GraphicsNode(this, goid, position, rotation, scale);
		}

		return _nodes[goid];
	}

	GraphicsNode * GraphicsManager::getGraphicsNode(const int64_t goid) const {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_nodes.find(goid) == _nodes.end()) {
			return nullptr;
		}

		return _nodes.find(goid)->second;
	}

	void GraphicsManager::deleteGraphicsNode(const int64_t goid) {
		ASSERT_THREAD_SAFETY_FUNCTION

		if (_nodes.find(goid) == _nodes.end()) {
			return;
		}

		delete _nodes[goid];

		_nodes.erase(_nodes.find(goid));
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
			std::string _heightmap = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->heightmap;
			std::string _texture = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->texture;
			double _size = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->size;

			addTerrain(msg->getContent()->getID(), boost::make_shared<Terrain>(this, _heightmap, _texture, _size));
		} else if (msg->getSubtype() == api::graphics::GraSkyBox) {
			api::graphics::Graphics_SkyBox_Create * c = static_cast<api::graphics::Graphics_SkyBox_Create *>(msg->getContent());

			_sceneManager->setSkyBox(true, c->material, c->distance, c->renderFirst);
		} else if (msg->getSubtype() == api::graphics::GraSkyDome) {
			api::graphics::Graphics_SkyDome_Create * c = static_cast<api::graphics::Graphics_SkyDome_Create *>(msg->getContent());

			_sceneManager->setSkyDome(true, c->material, c->curvature, c->tiles, c->distance, c->renderFirst);
		} else if (msg->getSubtype() == api::graphics::GraSkyPlane) {
			api::graphics::Graphics_SkyPlane_Create * c = static_cast<api::graphics::Graphics_SkyPlane_Create *>(msg->getContent());

			_sceneManager->setSkyPlane(true, Ogre::Plane(Ogre::Vector3(0.0, c->direction, 0.0), c->distance), c->material, c->size, c->tiles, c->renderFirst, c->curvature, c->xSegments, c->ySegments);
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

			_guiController->Mailbox(boost::make_shared<api::GameMessage>(api::messages::GUIMessageType, api::gui::GuiResolution, core::Method::Update, new api::gui::GUI_Resolution_Update(ru->resolution), i6engine::core::Subsystem::Graphic));
			api::EngineController::GetSingleton().getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::InputMessageType, api::input::InputResolution, core::Method::Update, new api::input::Input_Resolution_Update(ru->resolution), i6engine::core::Subsystem::Graphic));

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
			std::string _heightmap = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->heightmap;
			std::string _texture = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->texture;
			double _size = static_cast<api::graphics::Graphics_Terrain_Update *>(msg->getContent())->size;

			removeTerrain(msg->getContent()->getID());
			addTerrain(msg->getContent()->getID(), boost::make_shared<Terrain>(this, _heightmap, _texture, _size));
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
			GraphicsNode * node = getOrCreateGraphicsNode(goid);
			node->createCameraComponent(coid, m->pos, m->look, m->nearClip, m->fov);
		} else if (msg->getSubtype() == api::graphics::GraLuminous) {
			uint16_t t = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->lightType;

			api::LuminousAppearanceComponent::LightType lightType = static_cast<api::LuminousAppearanceComponent::LightType>(t);

			Vec3 diffuse = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->diffuse;
			Vec3 specular = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->specular;
			Vec4 attenuation = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->attenuation;
			Vec3 direction = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->direction;
			double spotLightRangeInner = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeInner;
			double spotLightRangeOuter = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->spotLightRangeOuter;
			Vec3 position = static_cast<api::graphics::Graphics_Luminous_Update *>(msg->getContent())->position;

			GraphicsNode * node = getOrCreateGraphicsNode(goid);
			node->createLuminousComponent(coid, lightType, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			std::string meshName = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->mesh;

			bool isVisible = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->visible;

			Vec3 p = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->pos;
			Quaternion r = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->rot;
			Vec3 s = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->scale;

			GraphicsNode * node = getOrCreateGraphicsNode(goid);
			node->getOrCreateSceneNode(coid, p, r, s);
			node->createMeshComponent(coid, meshName, isVisible);
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
			node->createOrUpdateViewport(coid, left, top, width, height, red, green, blue, alpha);
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			GraphicsNode * node = getOrCreateGraphicsNode(goid);

			Ogre::SceneNode * sceneNode = node->getSceneNode();

			Vec3 p = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->pos;

			sceneNode->setPosition(p.toOgre());
			// rotation
			Quaternion r = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->rot;

			sceneNode->setOrientation(r.toOgre());

			// Scale
			Vec3 s = static_cast<api::graphics::Graphics_Node_Update *>(msg->getContent())->scale;

			sceneNode->setScale(s.toOgre());
		} else if (msg->getSubtype() == api::graphics::GraParticle) {
			api::graphics::Graphics_Particle_Create * g = static_cast<api::graphics::Graphics_Particle_Create *>(msg->getContent());

			std::string emitterName = g->emitterName;
			Vec3 pos = g->pos;

			GraphicsNode * node = getOrCreateGraphicsNode(goid, pos);
			node->createParticleComponent(coid, emitterName, pos);
		} else if (msg->getSubtype() == api::graphics::GraBillboardSet) {
			api::graphics::Graphics_BillboardSet_Create * gbc = static_cast<api::graphics::Graphics_BillboardSet_Create *>(msg->getContent());

			GraphicsNode * node = getOrCreateGraphicsNode(goid, Vec3());
			node->createBilldboardSetComponent(coid, gbc->material, gbc->width, gbc->height, gbc->origin);
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
			node->updateCameraComponent(coid, m->pos, m->look, m->nearClip, m->fov);
		} else if (msg->getSubtype() == api::graphics::GraFrustum) {
			GraphicsNode * node = getGraphicsNode(goid);
			if (node == nullptr) {
				ISIXE_THROW_FAILURE("GraphicsManager", "Frustum message for invalid GameObject");
			}
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
			node->updateLuminousComponent(coid, lightType, diffuse, specular, attenuation, direction, position, spotLightRangeInner, spotLightRangeOuter);
		} else if (msg->getSubtype() == api::graphics::GraMaterial) {
			std::string materialName = static_cast<api::graphics::Graphics_Material_Update *>(msg->getContent())->material;

			GraphicsNode * node = getGraphicsNode(goid);

			if (node == nullptr) {
				return;
				ISIXE_THROW_MESSAGE("GraphicsManager", "No GraphicsNode to set material for");
			}

			node->setMaterial(coid, materialName);
		} else if (msg->getSubtype() == api::graphics::GraCusParam) {
			api::graphics::Graphics_CusParam_Update * m = static_cast<api::graphics::Graphics_CusParam_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);

			if (node == nullptr) {
				return;
				ISIXE_THROW_MESSAGE("GraphicsManager", "No GraphicsNode to set custom paramete for");
			}

			node->setCustomParameter(m->num, m->value);
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			std::string meshName = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->mesh;

			bool isVisible = static_cast<api::graphics::Graphics_Mesh_Create *>(msg->getContent())->visible;

			GraphicsNode * node = getGraphicsNode(goid);
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
			node->createOrUpdateViewport(coid, left, top, width, height, red, green, blue, alpha);
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node == nullptr) {
				return; // FIXME: (Daniel) happened after adding animations, but this musn't happen because of messaging system
			}

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

			if (node == nullptr) {
				return; // FIXME: (Daniel) musn't happen
			}

			api::graphics::Graphics_PlayAnimation_Update * gpu = dynamic_cast<api::graphics::Graphics_PlayAnimation_Update *>(msg->getContent());

			node->playAnimation(coid, gpu->anim, gpu->looping, gpu->offsetPercent);
		} else if (msg->getSubtype() == api::graphics::GraSetAnimationSpeed) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node == nullptr) {
				return; // FIXME: (Daniel) happened after adding animations, but this musn't happen because of messaging system
			}

			api::graphics::Graphics_SetAnimationSpeed_Update * gsu = dynamic_cast<api::graphics::Graphics_SetAnimationSpeed_Update *>(msg->getContent());

			node->setAnimationSpeed(gsu->speed);
		} else if (msg->getSubtype() == api::graphics::GraStopAnimation) {
			GraphicsNode * node = getGraphicsNode(goid);

			node->stopAnimation();
		} else if (msg->getSubtype() == api::graphics::GraBillboard) {
			api::graphics::Graphics_Billboard_Update * gbu = static_cast<api::graphics::Graphics_Billboard_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			if (node == nullptr) {
				return;
			}
			node->createOrUpdateBillboard(coid, gbu->identifier, gbu->offset, gbu->width, gbu->height, gbu->u0, gbu->v0, gbu->u1, gbu->v1);
		} else if (msg->getSubtype() == api::graphics::GraBillboardRemove) {
			api::graphics::Graphics_BillboardRemove_Update * gbu = static_cast<api::graphics::Graphics_BillboardRemove_Update *>(msg->getContent());

			GraphicsNode * node = getGraphicsNode(goid);
			node->deleteBillboard(coid, gbu->identifier);
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

			if (node != nullptr) {
				node->deleteCameraComponent(coid);
			}
		} else if (msg->getSubtype() == api::graphics::GraLuminous) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node != nullptr) {
				node->deleteLuminousComponent(coid);
			}
		} else if (msg->getSubtype() == api::graphics::GraMesh) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node != nullptr) {
				node->deleteMeshComponent(coid);
			}
		} else if (msg->getSubtype() == api::graphics::GraNode) {
			deleteGraphicsNode(coid);
		} else if (msg->getSubtype() == api::graphics::GraParticle) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node != nullptr) {
				node->deleteParticleComponent(coid);
			}
		} else if (msg->getSubtype() == api::graphics::GraBillboardSet) {
			GraphicsNode * node = getGraphicsNode(goid);

			if (node == nullptr) {
				return;
			}

			node->deleteBillboardSetComponent(coid);
		}
	}

	void GraphicsManager::addTicker(GraphicsNode * gn) {
		_tickers.insert(gn);
	}

	void GraphicsManager::removeTicker(GraphicsNode * gn) {
		_tickers.erase(gn);
	}

} /* namespace modules */
} /* namespace i6engine */
