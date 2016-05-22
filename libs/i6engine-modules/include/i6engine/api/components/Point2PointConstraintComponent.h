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

#ifndef __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__
#define __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class Point2PointConstraintComponent
	 * \brief Connects two PhysicalStateComponents in a point RELATIVE to their origin
	 * For creating a Point2PointConstraintComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | selfIdentifier | yes | std::string | identifier for this constraint component, must be unique over the complete game | yes |
	 * | targetIdentifier | yes | std::string | identifier of the constraint this component is connected to | yes |
	 * | selfOffset | yes | Vec3 | offset on which this constraint is connected relative to origin | yes |
	 * | targetOffset | yes | Vec3 | offset on which target constraint is connected relative to its origin | yes |
	 * | breakingImpulse | no | double | impulse that is necessary to break connection, not breakable if not set | yes |
	 */
	class ISIXE_MODULES_API Point2PointConstraintComponent : public Component {
	public:
		Point2PointConstraintComponent(const int64_t id, const attributeMap & params);
		~Point2PointConstraintComponent();

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "Point2PointConstraint";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		bool _connected;
		std::string _selfIdentifier;
		std::string _targetIdentifier;
		Vec3 _selfOffset;
		Vec3 _targetOffset;
		double _breakingImpulse;
		int64_t _targetID;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__ */

/**
 * @}
 */
