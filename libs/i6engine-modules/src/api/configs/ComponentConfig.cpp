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

#include "i6engine/api/configs/ComponentConfig.h"

namespace i6engine {
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

} /* namespace components */
} /* namespace api */
} /* namespace i6engine */

BOOST_CLASS_EXPORT_IMPLEMENT(i6engine::api::components::Component_Create_Create)
BOOST_CLASS_EXPORT_IMPLEMENT(i6engine::api::components::Component_Create_Delete)
BOOST_CLASS_EXPORT_IMPLEMENT(i6engine::api::components::Component_Spawnpoint_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6engine::api::components::Component_Shatter_Update)
BOOST_CLASS_EXPORT_IMPLEMENT(i6engine::api::components::Component_MoverResync_Update)
