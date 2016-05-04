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

#ifndef __I6ENGINE_API_LINECOMPONENT_H__
#define __I6ENGINE_API_LINECOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class LineComponent
	 * \brief creates a line in a given colour
	 * For creating a LineComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | startPos | yes | Vec3 | start position of the line | yes |
	 * | endPos | yes | Vec3 | end position of the line | yes |
	 * | colour | yes | Vec3 | colour of the line | yes |
	 */
	class ISIXE_MODULES_API LineComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 *
		 * Sets visibility and ID
		 */
		LineComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Empty
		 *
		 * Hasn't to do anything right now
		 */
		~LineComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Line";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		Vec3 _startPos;
		Vec3 _endPos;
		Vec3 _colour;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_LINECOMPONENT_H__ */

/**
 * @}
 */
