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

#ifndef __I6ENGINE_EDITOR_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__
#define __I6ENGINE_EDITOR_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__

#include "i6engine/api/components/Component.h"
#include "i6engine/api/facades/MessageSubscriberFacade.h"

namespace i6engine {
namespace editor {
namespace components {

	class ISIXE_EDITOR_API ToggleWaynetComponent : public api::Component, public api::MessageSubscriberFacade {
	public:
		ToggleWaynetComponent(int64_t id, const api::attributeMap & params);

		static api::ComPtr createC(int64_t id, const api::attributeMap & params);

		void Init() override;

		void Tick() override;

		void Finalize() override;

		api::attributeMap synchronize() const {
			return {};
		}

		std::string getTemplateName() const {
			return "ToggleWaynet";
		}

		std::vector<api::componentOptions> getComponentOptions() {
			return {};
		}

		void News(const api::GameMessage::Ptr & msg) override;

	private:
		bool _active;
	};

} /* namespace components */
} /* namespace editor */
} /* namespace i6engine */

#endif /* __I6ENGINE_EDITOR_COMPONENTS_TOGGLEWAYNETCOMPONENT_H__ */
