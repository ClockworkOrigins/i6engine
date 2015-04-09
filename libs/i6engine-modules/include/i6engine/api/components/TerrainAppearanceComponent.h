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

#ifndef __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__
#define __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6engine {
namespace api {

	/**
	 * \class TerrainAppearanceComponent
	 * \brief Tells the engine what heightmap and texture to use when rendering the GameObject.
	 */
	class ISIXE_MODULES_API TerrainAppearanceComponent : public Component {
	public:
		TerrainAppearanceComponent(const int64_t id, const attributeMap & params);
		virtual ~TerrainAppearanceComponent();

		/**
		* \brief creates the Component with given attributeMap
		*/
		static ComPtr createC(const int64_t id, const attributeMap & params);

		void Init() override;

		/**
		 * \brief Gets the GameObject's heightmap.
		 */
		inline std::string getHeightmap() const { return _heightmap; }

		/**
		 * \brief Gets the GameObject's size.
		 */
		inline double getSize() const { return _size; }

		/**
		 * \brief synchronizes the Components state
		 */
		attributeMap synchronize() const override;

		std::string getTemplateName() const override {
			return "TerrainAppearance";
		}

		std::vector<componentOptions> getComponentOptions() override;

	private:
		std::string _heightmap;
		double _size;
		double _inputScale;
		std::vector<std::tuple<double, std::string, std::string>> _layers;

		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__ */

/**
 * @}
 */
