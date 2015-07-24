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

#ifndef __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__
#define __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__

#include "i6engine/math/i6eQuaternion.h"
#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {
namespace graphics {
	enum class BillboardOrigin;
} /* namespace graphics */

	/**
	 * \class MovableTextComponent
	 * \brief shows a text above a mesh
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
		 * \brief creates the Component with given attributeMap
		 */
		static ComPtr createC(const int64_t id, const attributeMap & params);

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

	private:
		std::string _font;
		std::string _text;
		uint16_t _size;
		Vec3 _colour;

		void Init() override;

		void Finalize() override;

		std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_MOVABLETEXTCOMPONENT_H__ */

/**
 * @}
 */
