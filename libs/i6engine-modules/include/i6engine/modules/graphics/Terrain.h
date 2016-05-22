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
 * \addtogroup Graphics
 * @{
 */

#ifndef __I6ENGINE_MODULES_TERRAIN_H__
#define __I6ENGINE_MODULES_TERRAIN_H__

#include <cstdint>
#include <string>
#include <vector>

#include "i6engine/utils/i6eThreadSafety.h"

namespace Ogre {
	class Image;
	class Terrain;
	class TerrainGroup;
	class TerrainGlobalOptions;
} /* namespace Ogre */

namespace i6e {
namespace modules {

	class GraphicsManager;

	/**
	 * \ingroup Graphic
	 *
	 * \class Terrain
	 * \brief Saves all Terrain-specific things
	 */
	class ISIXE_MODULES_API Terrain {
	public:
		/**
		 * \brief Creates a new Terrain
		 * Creates a new Ogre terrain by using Ogre's TerrainGroup object with a heightmap
		 * Thereby passing a filename and calling Ogre's loadAllTerrains
		 * Then configuring
		 */
		Terrain(GraphicsManager * manager, const std::string & heightmap, const double size, double inputScale, uint16_t vertices, const std::vector<std::tuple<double, std::string, std::string, double, double>> & layers, int64_t minX, int64_t minY, int64_t maxX, int64_t maxY);
		
		/**
		 * \brief Creates a new Terrain
		 * Creates a new Ogre terrain by using Ogre's TerrainGroup object with heightdata
		 * Thereby passing a filename and calling Ogre's loadAllTerrains
		 * Then configuring
		 */
		Terrain(GraphicsManager * manager, const std::vector<std::vector<double>> & heightdata, const double size, double inputScale, uint16_t vertices, const std::vector<std::tuple<double, std::string, std::string, double, double>> & layers, int64_t minX, int64_t minY, int64_t maxX, int64_t maxY);

		/**
		 * \brief Terrain::~Terrain()
		 * Calls Ogre's terrain object delete functions
		 */
		~Terrain();

		/**
		 * \brief Configures Ogre Terrain using heightmap
		 */
		void defineTerrainHeightmap(const int64_t x, const int64_t y);

		/**
		 * \brief Configures Ogre Terrain using heightdata
		 */
		void defineTerrainHeightdata(const int64_t x, const int64_t y);

		/**
		 * \brief Terrain::configureTerrainDefaults()
		 * Configures Ogre Terrain using default values
		 */
		void configureTerrainDefaults();

		/**
		 * \brief Terrain::getTerrainImage(const bool flipX, const bool flipY, Ogre::Image & img)
		 * loads an Ogre terain image and flips it depending on flipX and flipY params.
		 */
		void getTerrainImage(const bool flipX, const bool flipY, Ogre::Image & img);

		void initBlendMaps(Ogre::Terrain * terrain);

		/**
		 * \brief Returns a pointer to the TerrainGroup of the graphics library
		 * Returns a pointer to the TerrainGroup of the graphics library
		 */
		inline Ogre::TerrainGroup * getTerrainGroup() const { return _mTerrainGroup; }

		/**
		 * \brief Returns a pointer to the TerrainGlobalsOptions of the graphics library
		 * Returns a pointer to the TerrainGlobalOptions of the graphics library
		 */
		inline Ogre::TerrainGlobalOptions * getTerrainGlobalOptions() const { return _mTerrainGlobals; }

		/**
		 * \brief sets height at position
		 */
		void setHeightAtPosition(uint64_t x, uint64_t z, double height);

		/**
		 * \brief saves collision shape as bullet collisionshape to given file
		 */
		void saveCollisionShape(const std::string & outFile);

		bool isDirty() const {
			return _dirty;
		}

		void update();

	private:
		/**
		 * \brief pointer to the graphics subsystem
		 */
		GraphicsManager * _manager;

		/**
		 * TerrainGroup of the graphics library
		 */
		Ogre::TerrainGroup * _mTerrainGroup;

		/**
		 * TerrainGlobalOptions of the graphics library
		 */
		Ogre::TerrainGlobalOptions * _mTerrainGlobals;

		/**
		 * TerrainsImported???
		 */
		bool _mTerrainsImported;

		/**
		 * Heightmap of the terrain
		 */
		std::string _heightmap;

		/**
		 * height data of the terrain
		 */
		std::vector<std::vector<double>> _heightdata;

		/**
		 * Size of the terrain
		 */
		double _size;

		double _inputScale;

		uint16_t _vertices;

		std::vector<std::tuple<double, std::string, std::string, double, double>> _layers;

		int64_t _minX;
		int64_t _maxX;
		int64_t _minY;
		int64_t _maxY;

		bool _dirty;

		/**
		 * \brief forbidden
		 */
		Terrain(const Terrain &) = delete;

		/**
		 * \brief forbidden
		 */
		const Terrain & operator=(const Terrain &) = delete;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_TERRAIN_H__ */

/**
 * @}
 */
