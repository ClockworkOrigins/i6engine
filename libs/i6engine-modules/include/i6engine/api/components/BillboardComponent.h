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

/**
 * \addtogroup Components
 * @{
 */

#ifndef __I6ENGINE_API_BILLBOARDCOMPONENT_H__
#define __I6ENGINE_API_BILLBOARDCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
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
} /* namespace i6e */

#endif /* __I6ENGINE_API_BillboardComponent_H__ */

/**
 * @}
 */
