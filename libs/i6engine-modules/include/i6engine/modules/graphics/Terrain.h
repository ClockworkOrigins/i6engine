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

namespace i6engine {
namespace modules {

	class GraphicsManager;

	/**
	 * \ingroup Graphic
	 *
	 * \class Terrain
	 * \brief Saves all Terrain-specific things
	 */
	class Terrain {
	public:
		/**
		 * \brief Creates a new node with unique name
		 *
		 *     Creates a new Ogre terrain by using Ogre's TerrainGroup object
		 *     Thereby passing a filename and calling Ogre's loadAllTerrains
		 *     Then configuring
		 *
		 * \param[in] goid Timestamp
		 * \return   nothing
		 *
		 */
		Terrain(GraphicsManager * manager, const std::string & heightmap, const double size, double inputScale, const std::vector<std::tuple<double, std::string, std::string, double, double>> & layers);

		/**
		 * \brief Terrain::~Terrain()
		 *
		 *     Calls Ogre's terrain object delete functions
		 *
		 * \param   no params
		 * \return   nothing
		 *
		 */
		~Terrain();

		/**
		 * \brief Terrain::defineTerrain(const long x, const long y)
		 *
		 *     Configures Ogre Terrain texture (?)
		 *
		 * \param   const long x, const long y
		 * \return   nothing
		 *
		 */
		void defineTerrain(const int64_t x, const int64_t y);

		/**
		 * \brief Terrain::configureTerrainDefaults()
		 *
		 *     Configures Ogre Terrain using default values
		 *
		 * \param   no params
		 * \return   nothing
		 *
		 */
		void configureTerrainDefaults();

		/**
		 * \brief Terrain::getTerrainImage(const bool flipX, const bool flipY, Ogre::Image & img)
		 *
		 *     loads an Ogre terain image and flips it depending on flipX and flipY params.
		 *
		 * \param   const bool flipX, const bool flipY, Ogre::Image & img
		 * \return   nothing
		 *
		 */
		void getTerrainImage(const bool flipX, const bool flipY, Ogre::Image & img);

		void initBlendMaps(Ogre::Terrain * terrain);

		/**
		 * \brief Returns a pointer to the TerrainGroup of the graphics library
		 *
		 * Returns a pointer to the TerrainGroup of the graphics library
		 *
		 * \return TerrainGroup of the object
		 */
		inline Ogre::TerrainGroup * getTerrainGroup() const { return _mTerrainGroup; }

		/**
		 * \brief Returns a pointer to the TerrainGlobalsOptions of the graphics library
		 *
		 * Returns a pointer to the TerrainGlobalOptions of the graphics library
		 *
		 * \return TerrainGlobalOptions of the object
		 */
		inline Ogre::TerrainGlobalOptions * getTerrainGlobalOptions() const { return _mTerrainGlobals; }

	private:
		/**
		 * \brief forbidden
		 */
		Terrain(const Terrain &) = delete;

		/**
		 * \brief forbidden
		 */
		const Terrain & operator=(const Terrain &) = delete;

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
		 * Size of the terrain
		 */
		double _size;

		double _inputScale;

		std::vector<std::tuple<double, std::string, std::string, double, double>> _layers;

		ASSERT_THREAD_SAFETY_HEADER
	};

} /* namespace modules */
} /* namespace i6engine */

#endif /* __I6ENGINE_MODULES_TERRAIN_H__ */

/**
 * @}
 */
