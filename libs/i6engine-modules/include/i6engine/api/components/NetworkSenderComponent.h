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

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_NETWORKSENDERCOMPONENT_H__
#define __I6ENGINE_API_NETWORKSENDERCOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \brief defines that this gameobject shall be synchronised over network
	 */
	class ISIXE_MODULES_API NetworkSenderComponent : public Component {
	public:
		NetworkSenderComponent();
		~NetworkSenderComponent();

		void Init() override;

		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "NetworkSender";
		}

		std::vector<componentOptions> getComponentOptions() override {
			return {};
		}
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_NETWORKSENDERCOMPONENT_H__ */

/**
 * @}
 */
