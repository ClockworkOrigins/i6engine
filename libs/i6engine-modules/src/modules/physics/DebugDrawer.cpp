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

#include "i6engine/modules/physics/DebugDrawer.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6engine {
namespace modules {

	void DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & col) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GraphicsMessageType, api::graphics::GraLine, core::Method::Create, new api::graphics::Graphics_Line_Create(Vec3(from), Vec3(to), Vec3(col)), core::Subsystem::Unknown));
	}

	void DebugDrawer::drawContactPoint(const btVector3 & a, const btVector3 & b, btScalar g, int f, const btVector3 & c) {
	}

	void DebugDrawer::reportErrorWarning(const char * d) {
	}

	void DebugDrawer::draw3dText(const btVector3 & a, const char * c) {
	}

	void DebugDrawer::setDebugMode(int a) {
	}

	int DebugDrawer::getDebugMode() const {
		return btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;
	}

} /* namespace modules */
} /* namespace i6engine */
