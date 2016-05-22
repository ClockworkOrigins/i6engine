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

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6e {
namespace api {
namespace components {

	Component_Create_Create::Component_Create_Create() : GameMessageStruct() {
	}
	Component_Create_Create::Component_Create_Create(const int64_t goid, const int64_t coid, const core::IPKey & r, const std::string & t, const api::attributeMap & p) : GameMessageStruct(coid, goid), tpl(t), params(p), receiver(r) {
	}

	Component_CreateCallback_Create::Component_CreateCallback_Create(const int64_t goid, const int64_t coid, const core::IPKey & r, const std::string & t, const api::attributeMap & p, const boost::function<void(ComPtr)> & cb) : GameMessageStruct(coid, goid), tpl(t), params(p), receiver(r), callback(cb) {
	}

	Component_Create_Delete::Component_Create_Delete() : GameMessageStruct() {
	}
	Component_Create_Delete::Component_Create_Delete(const int64_t goid, const int64_t coid, uint32_t fam) : GameMessageStruct(coid, goid), famID(fam) {
	}

	Component_Spawnpoint_Update::Component_Spawnpoint_Update() : GameMessageStruct() {
	}
	Component_Spawnpoint_Update::Component_Spawnpoint_Update(const int64_t goid, const int64_t coid) : GameMessageStruct(coid, goid) {
	}

	Component_Shatter_Update::Component_Shatter_Update() : GameMessageStruct() {
	}
	Component_Shatter_Update::Component_Shatter_Update(const int64_t goid, const int64_t coid, const int64_t o, ShatterInterest si) : GameMessageStruct(coid, goid), other(o), shatterInterest(si) {
	}

	Component_MoverResync_Update::Component_MoverResync_Update() : GameMessageStruct() {
	}
	Component_MoverResync_Update::Component_MoverResync_Update(const int64_t goid, const int64_t coid, std::map<std::string, std::string> am) : GameMessageStruct(coid, goid), attMap(am) {
	}

	Component_EnableTicking_Update::Component_EnableTicking_Update(const int64_t goid, const int64_t coid, bool at) : GameMessageStruct(coid, goid), allowTicking(at) {
	}

} /* namespace components */
} /* namespace api */
} /* namespace i6e */

BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::components::Component_Create_Create)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::components::Component_Create_Delete)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::components::Component_Spawnpoint_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::components::Component_Shatter_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6e::api::components::Component_MoverResync_Update)
