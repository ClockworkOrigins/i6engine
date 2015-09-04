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

#ifndef __I6ENGINE_API_BILLBOARDCOMPONENT_H__
#define __I6ENGINE_API_BILLBOARDCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {
namespace graphics {
	enum class BillboardOrigin;
} /* namespace graphics */

	/**
	 * \class BillboardComponent
	 * \brief Tells the engine what billboard to show
	 * For creating a BillboardComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | material | yes | std::string | Material used for the Billboard | yes |
	 * | width | yes | double | relative width | yes |
	 * | height | yes | double | relative height | yes |
	 * | origin | yes | int | one of BillboardOrigin's int values | yes |
	 */
	class ISIXE_MODULES_API BillboardComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 */
		BillboardComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~BillboardComponent();

		/**
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

		/**
		 * \brief creates or updates a billboard in this billboard set
		 */
		void createOrUpdateBillboard(const std::string & identifier, const Vec3 & offset, double width, double height, double u0, double v0, double u1, double v1);

		/**
		 * \brief deletes a billboard
		 */
		void deleteBillboard(const std::string & identifier);

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Billboard";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		std::string _material;
		double _width;
		double _height;
		graphics::BillboardOrigin _billboardOrigin;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_BillboardComponent_H__ */

/**
 * @}
 */
