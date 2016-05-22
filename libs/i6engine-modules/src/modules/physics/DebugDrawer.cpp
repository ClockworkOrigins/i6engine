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

#include "i6engine/modules/physics/DebugDrawer.h"

#include "i6engine/core/configs/SubsystemConfig.h"

#include "i6engine/api/FrontendMessageTypes.h"
#include "i6engine/api/configs/GraphicsConfig.h"
#include "i6engine/api/facades/MessagingFacade.h"

namespace i6e {
namespace modules {

	void DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & col) {
		api::EngineController::GetSingletonPtr()->getMessagingFacade()->deliverMessage(boost::make_shared<api::GameMessage>(api::messages::GraphicsMessageType, api::graphics::GraLine, core::Method::Create, new api::graphics::Graphics_Line_Create(Vec3(from), Vec3(to), Vec3(col)), core::Subsystem::Unknown));
	}

	void DebugDrawer::drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {
	}

	void DebugDrawer::reportErrorWarning(const char *) {
	}

	void DebugDrawer::draw3dText(const btVector3 &, const char *) {
	}

	void DebugDrawer::setDebugMode(int) {
	}

	int DebugDrawer::getDebugMode() const {
		return btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;
	}

} /* namespace modules */
} /* namespace i6e */
