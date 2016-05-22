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
