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

#ifndef __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__
#define __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class MovableTextComponent
	 * \brief shows a text above a mesh
	 * For creating a MovableTextComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | font | yes | std::string | font used for this Text | yes |
	 * | text | yes | std::string | the text shown above the GameObject | yes |
	 * | size | yes | double | font size | yes |
	 * | colour | yes | Vec3 | colour of the text | yes |
	 */
	class ISIXE_MODULES_API MovableTextComponent : public Component {
	public:
		/**
		 * \brief Constructor of the component
		 */
		MovableTextComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		~MovableTextComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "MovableText";
		}

		std::vector<componentOptions> getComponentOptions() override;

		/**
		 * \brief sets the text
		 */
		void setText(const std::string & text);

		void setAutoScaleCallback(const std::function<double(const Vec3 &, const Vec3 &)> & callback);

	private:
		std::string _font;
		std::string _text;
		double _size;
		Vec3 _colour;
		std::function<double(const Vec3 &, const Vec3 &)> _autoScaleCallback;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__ */

/**
 * @}
 */
