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

#include "i6engine/api/facades/GraphicsFacade.h"

#include "i6engine/utils/i6eString.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace api {

	GraphicsFacade::GraphicsFacade() : _notify(), _notifyInit(false), _resolutions(), _currentResolution(), _selectablesLock(), _selectables() {
		clockUtils::iniParser::IniParser ip2;
		ip2.load("i6engine.ini");
		std::string ogrePath;
		if (clockUtils::ClockError::SUCCESS != ip2.getValue<std::string>("GRAPHIC", "ogreConfigsPath", ogrePath)) {
			return;
		}

		clockUtils::iniParser::IniParser ip;
		ip.load(ogrePath + "/ogre.cfg");

		std::string res;
		if (clockUtils::ClockError::SUCCESS != ip.getValue("OpenGL Rendering Subsystem", "Video Mode", res)) {
			return;
		}

		std::vector<std::string> resVec = i6engine::utils::split(res, " ");
		
		if (resVec.size() != 3) {
			return;
		}

		_currentResolution.width = uint32_t(std::atoi(resVec.front().c_str()));
		_currentResolution.height = uint32_t(std::atoi(resVec.back().c_str()));
	}

	GraphicsFacade::~GraphicsFacade() {
	}

	void GraphicsFacade::removeNode(const int64_t id) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsNodeMessageType, graphics::GraNode, core::Method::Delete, new graphics::Graphics_Node_Delete(id), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::closeViewports() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraViewports, core::Method::Delete, new graphics::Graphics_Viewports_Delete(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setAmbientLight(double r, double g, double b) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraAmbLight, core::Method::Update, new graphics::Graphics_AmbLight_Update(r, g, b), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setSkyBox(const std::string & material, double distance, bool renderFirst) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyBox, core::Method::Create, new graphics::Graphics_SkyBox_Create(material, distance, renderFirst), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setSkyDome(const std::string & material, int32_t curvature, double tiles, double distance, bool renderFirst) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyDome, core::Method::Create, new graphics::Graphics_SkyDome_Create(material, curvature, tiles, distance, renderFirst), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setSkyPlane(const std::string & material, double distance, int32_t direction, int32_t size, double tiles, bool renderFirst, double curvature, int32_t xSegments, int32_t ySegments) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyPlane, core::Method::Create, new graphics::Graphics_SkyPlane_Create(material, distance, direction, size, tiles, renderFirst, curvature, xSegments, ySegments), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::removeSkyBox() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyBox, core::Method::Delete, new graphics::Graphics_SkyBox_Delete(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::removeSkyDome() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyDome, core::Method::Delete, new graphics::Graphics_SkyDome_Delete(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::removeSkyPlane() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSkyPlane, core::Method::Delete, new graphics::Graphics_SkyPlane_Delete(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setResolution(const graphics::Resolution & resolution) {
		_currentResolution = resolution;
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraResolution, core::Method::Update, new graphics::Graphics_Resolution_Update(resolution), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setFullscreen(bool fullscreen) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraFullscreen, core::Method::Update, new graphics::Graphics_Fullscreen_Update(fullscreen), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setShadowTechnique(graphics::ShadowTechnique st) {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraShadowTechnique, core::Method::Update, new graphics::Graphics_ShadowTechnique_Update(st), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

	void GraphicsFacade::setLinearFog(const Vec3 & colour, double start, double end) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSetLinearFog, core::Method::Update, new graphics::Graphics_SetLinearFog_Update(colour, start, end), core::Subsystem::Unknown));
	}

	void GraphicsFacade::setExponentialFog(const Vec3 & colour, double density) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSetExponentialFog, core::Method::Update, new graphics::Graphics_SetExponentialFog_Update(colour, density), core::Subsystem::Unknown));
	}

	void GraphicsFacade::setExponentialFog2(const Vec3 & colour, double density) {
		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraSetExponentialFog2, core::Method::Update, new graphics::Graphics_SetExponentialFog_Update(colour, density), core::Subsystem::Unknown));
	}

	void GraphicsFacade::registerNotifyCallback(const boost::function<void(int64_t)> & f) {
		_notify = f;
		_notifyInit = true;
	}

	void GraphicsFacade::notifyNewID(int64_t id) {
		if (_notifyInit) {
			_notify(id);
		}
	}

	void GraphicsFacade::resetSubSystem() {
		GameMessage::Ptr msg = boost::make_shared<GameMessage>(messages::GraphicsMessageType, graphics::GraReset, core::Method::Delete, new GameMessageStruct(), core::Subsystem::Unknown);

		EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(msg);
	}

} /* namespace api */
} /* namespace i6engine */
