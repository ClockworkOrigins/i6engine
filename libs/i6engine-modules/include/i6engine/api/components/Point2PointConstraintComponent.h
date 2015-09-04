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

#ifndef __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__
#define __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
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
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

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
} /* namespace i6engine */

#endif /* __I6ENGINE_API_POINT2POINTCONSTRAINTCOMPONENT_H__ */

/**
 * @}
 */
