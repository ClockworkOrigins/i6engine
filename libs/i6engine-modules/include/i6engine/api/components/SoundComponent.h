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

#ifndef __I6ENGINE_API_SOUNDCOMPONENT_H__
#define __I6ENGINE_API_SOUNDCOMPONENT_H__

#include "i6engine/math/i6eVector.h"

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	class PhysicalStateComponent;

	/**
	 * \class SoundComponent
	 * \brief Attaches a sound node to an object. The Sound will follow the object
	 */
	class ISIXE_MODULES_API SoundComponent : public Component {
	public:
		/**
		 * \brief Constructor with attribute map
		 */
		SoundComponent(const int64_t id, const attributeMap & params);

		/**
		 * \brief Destructor
		 */
		virtual ~SoundComponent();

		static ComPtr createC(int64_t id, const attributeMap & params);

		/**
		 * \brief initializes the component
		 * \info don't use this method at the moment, because all properties will be random
		 */
		void Init() override;

		void Finalize() override;

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() override;

		virtual std::pair<AddStrategy, int64_t> howToAdd(const ComPtr & comp) const override;

		std::string getTemplateName() const override {
			return "Sound";
		}

		void Tick() override;

		std::vector<componentOptions> getComponentOptions() override;

	private:
		/**
		 * \brief Position of the parent
		 */
		Vec3 _position;

		/**
		 * \brief Offset of the sound of the GameObject itself
		 */
		Vec3 _offset;

		/**
		 * \brief direction of the sound
		 */
		Vec3 _direction;

		/**
		 * \brief Filename of the Sound
		 */
		std::string _file;

		/**
		 * \brief is the sound looping or not?
		 */
		bool _looping;

		/**
		 * \brief distance where the sound disappears
		 */
		double _maxDist;

		/**
		 * \brief weakPtr on PhysicalStateComponent for faster access
		 */
		utils::weakPtr<PhysicalStateComponent, Component> _psc;

		/**
		 * \brief will this sound file be cached
		 */
		bool _cacheable;
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_SOUNDCOMPONENT_H__ */

/**
 * @}
 */
