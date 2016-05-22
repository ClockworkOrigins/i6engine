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

#ifndef __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__
#define __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__

#include "i6engine/api/components/Component.h"

namespace i6e {
namespace api {

	/**
	 * \class TerrainAppearanceComponent
	 * \brief Tells the engine what heightmap and texture to use when rendering the GameObject.
	 * For creating a TerrainAppearanceComponent, these keys are possible:
	 * | Name | Required | Type | Description | Public |
	 * |------|----------|------| ----------- | ------------ |
	 * | heightmap | yes | std::string | heightmap used for this terrain, either this or heightdata required, if both are provided, heightdata is used | yes |
	 * | heightdata | yes | std::vector<std::vector<double>> | heightdata used for this terrain, either this or heightmap required, if both are provided, heightdata is used | yes |
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
		 * \brief sets the height at a defined position in the terrain
		 * coordinates in vertices
		 */
		void setHeightAtPosition(size_t x, size_t z, double height);

		/**
		 * \brief sets the height at a defined position in the terrain
		 * position is a real world position
		 */
		void setHeightAtPosition(const Vec3 & pos, double height);
		
		/**
		 * \brief gets the height at a defined position in the terrain
		 * coordinates in vertices
		 */
		double getHeightAtPosition(size_t x, size_t z) const;
		
		/**
		 * \brief gets the height at a defined position in the terrain
		 * position is a real world position
		 */
		double getHeightAtPosition(const Vec3 & pos) const;

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
		std::vector<std::vector<double>> _heightdata;
		double _size;
		double _inputScale;
		uint16_t _vertices;
		std::vector<std::tuple<double, std::string, std::string, double, double>> _layers;
		int64_t _minX;
		int64_t _maxX;
		int64_t _minY;
		int64_t _maxY;

		void Init() override;

		void Finalize() override;

		void sendUpdateMessage();

		void convertPositionToVertex(const Vec3 & pos, size_t & x, size_t & z) const;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_TERRAINAPPEARANCECOMPONENT_H__ */

/**
 * @}
 */
