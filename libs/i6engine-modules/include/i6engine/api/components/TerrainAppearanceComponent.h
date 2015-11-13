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
	 * For creating a TerrainAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | heightmap | yes | std::string | heightmap used for this terrain | yes |
	 * | size | yes | double | expansion of the terrain | yes |
	 * | inputScale | yes | double | scale factor for the terrain heights, highest point of heightmap gets this height | yes |
	 * | vertices | yes | uint32_t | vertices along an edge, must be 2^n + 1 | yes |
	 * | layers | yes | uint32_t | amount of layers on the terrain | yes |
	 * | minX | yes | int64_t | minimum X for duplicates of the terrain | yes |
	 * | maxX | yes | int64_t | maximum X for duplicates of the terrain | yes |
	 * | minY | yes | int64_t | maximum Y for duplicates of the terrain | yes |
	 * | maxY | yes | int64_t | maximum Y for duplicates of the terrain | yes |
	 * | layer_<i>_size | yes | double | size of the texture units for every layer | yes |
	 * | layer_<i>_diffusespecular | yes | std::string | diffuse and specular map for every layer | yes |
	 * | layer_<i>_normal | yes | std::string | normal and displacement map for every layer | yes |
	 * | layer_<i>_minHeight | yes | double | minimum height where this layer shall appear, for every layer except the first | yes |
	 * | layer_<i>_fadeDist | yes | double | defines how fast the layers are blend over, for every layer except the first | yes |
	 */
	class ISIXE_MODULES_API TerrainAppearanceComponent : public Component {
	public:
		TerrainAppearanceComponent(const int64_t id, const attributeMap & params);
		~TerrainAppearanceComponent();

		/**
		 * \brief Gets the GameObject's heightmap.
		 */
		inline std::string getHeightmap() const { return _heightmap; }

		/**
		 * \brief Gets the GameObject's size.
		 */
		inline double getSize() const { return _size; }

		/**
		 * \brief saves collision shape as bullet collisionshape to given file
		 * calls callback after collision shape was exported
		 */
		void saveCollisionShape(const std::string & outFile, const std::function<void(void)> & cb);

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
		uint32_t _vertices;
		std::vector<std::tuple<double, std::string, std::string, double, double>> _layers;
		int64_t _minX;
		int64_t _maxX;
		int64_t _minY;
		int64_t _maxY;

		void Init() override;

		void sendUpdateMessage();
	};

} /* namespace api */
} /* namespace i6engine */

#endif /* __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__ */

/**
 * @}
 */
