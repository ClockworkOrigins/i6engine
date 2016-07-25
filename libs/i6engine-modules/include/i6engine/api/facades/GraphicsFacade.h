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

#ifndef __I6ENGINE_API_GRAPHICSFACADE_H__
#define __I6ENGINE_API_GRAPHICSFACADE_H__

#include <mutex>

#include "i6engine/utils/i6eSystemParameters.h"

#include "i6engine/api/configs/GraphicsConfig.h"

#include "boost/function.hpp"

namespace i6e {
namespace api {

	class ISIXE_MODULES_API GraphicsFacade {
	public:
		GraphicsFacade();

		~GraphicsFacade();

		/**
		 * \brief closes all viewports of the scene
		 */
		void closeViewports();

		/**
		 * \brief sets the color for the ambient light of the scene
		 */
		void setAmbientLight(double r, double g, double b);

		/**
		 * \brief adds a sky box to the world
		 */
		void setSkyBox(const std::string & material, double distance = 5000.0, bool renderFirst = true);
		void setSkyDome(const std::string & material, int32_t curvature = 10, double tiles = 8.0, double distance = 4000.0, bool renderFirst = true);
		void setSkyPlane(const std::string & material, double distance, int32_t direction, int32_t size = 1000, double tiles = 10.0, bool renderFirst = true, double curvature = 0.0, int32_t xSegments = 1, int32_t ySegments = 1);

		void removeSkyBox();
		void removeSkyDome();
		void removeSkyPlane();

		/**
		 * \brief changes resolution to given one
		 */
		void setResolution(const graphics::Resolution & resolution);

		/**
		 * \brief changes fullscreen mode of window
		 */
		void setFullscreen(bool fullscreen);

		/**
		 * \brief sets the shadow technique
		 */
		void setShadowTechnique(graphics::ShadowTechnique st);

		/**
		 * \brief register callback for notifying IDs, call only from GraphicsController
		 */
		void registerNotifyCallback(const std::function<void(int64_t)> & f);

		/**
		 * \brief notifies new id in subsystem
		 */
		void notifyNewID(int64_t id);

		/**
		 * \brief resets the subsystem to it's defaults
		 */
		void resetSubSystem();

		/**
		 * \brief sets possible resolutions, called by graphics subsystem
		 */
		void setPossibleResolutions(const std::vector<graphics::Resolution> & resolutions) {
			_resolutions = resolutions;
		}

		/**
		 * \brief returns all resolutions the graphics subsystem can show
		 */
		std::vector<graphics::Resolution> getPossibleResolutions() const {
			return _resolutions;
		}

		/**
		 * \brief returns current resolution
		 */
		graphics::Resolution getCurrentResolution() const {
			return _currentResolution;
		}

		/**
		 * \brief sets the selectables, only used by graphics
		 */
		void setSelectables(const std::vector<std::pair<int64_t, Vec3>> selectables) {
			std::lock_guard<std::mutex> lg(_selectablesLock);
			_selectables = selectables;
		}

		/**
		 * \brief returns the selectables
		 */
		std::vector<std::pair<int64_t, Vec3>> getSelectables() const {
			std::lock_guard<std::mutex> lg(_selectablesLock);
			return _selectables;
		}

		/**
		 * \brief sets linear fog
		 * \param colour colour of the fog
		 * \param start the distance the fog starts
		 * \param end the distance the fog is complete thick
		 */
		void setLinearFog(const Vec3 & colour, double start, double end) const;

		/**
		 * \brief sets exponential fog
		 * \param colour colour of the fog
		 * \param density the density of the fog
		 */
		void setExponentialFog(const Vec3 & colour, double density) const;

		/**
		 * \brief sets exponential fog (2, faster increase of density)
		 * \param colour colour of the fog
		 * \param density the density of the fog
		 */
		void setExponentialFog2(const Vec3 & colour, double density) const;

		/**
		 * \brief make a screenshot of current scene using prefix, timestamp and suffix
		 * \param prefix prefix of the file
		 * \param suffix suffix of the file, something like .bmp
		 */
		void takeScreenshot(const std::string & prefix, const std::string & suffix) const;

		/**
		 * \brief shows graphic frames
		 * \param x x position of the frames window
		 * \param y y position of the frames window
		 * \param imageStyle style of the image
		 * \param printStyle style of the print
		 * \param imageset imageset of the frame window
		 * \param image image of the frame window
		 */
		void showFPS(double x, double y, const std::string & imageStyle, const std::string & printStyle, const std::string & imageset, const std::string & image) const;

		/**
		 * \brief hides graphic frames
		 */
		void hideFPS() const;

		/**
		 * \brief calls callback with highest position of a mesh
		 */
		void getHighestCoordinate(const Vec3 & startPos, const std::function<void(Vec3)> & callback) const;

	private:
		std::function<void(int64_t)> _notify;

		bool _notifyInit;

		std::vector<graphics::Resolution> _resolutions;

		graphics::Resolution _currentResolution;

		mutable std::mutex _selectablesLock;
		std::vector<std::pair<int64_t, Vec3>> _selectables;

		/**
		 * \brief forbidden
		 */
		GraphicsFacade(const GraphicsFacade &) = delete;

		/**
		 * \brief forbidden
		 */
		const GraphicsFacade & operator=(const GraphicsFacade &) = delete;
	};

} /* namespace api */
} /* namespace i6e */

#endif /* __I6ENGINE_API_GRAPHICSFACADE_H__ */

/**
 * @}
 */
