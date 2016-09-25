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
 * \addtogroup GUI
 * @{
 */

#ifndef __I6ENGINE_MODULES_GUIIMAGE_H__
#define __I6ENGINE_MODULES_GUIIMAGE_H__

#include "i6engine/api/gui/GUIWidget.h"

namespace i6e {
namespace modules {

	class GUIImage : public api::GUIWidget {
	public:
		/**
		 * \brief Constructor for a new GUI Image.
		 * \param name Name of the new Object.
		 * \param type Type of the new Object.
		 */
		GUIImage(const std::string & name, const std::string & type);

		/**
		 * \brief destructor
		 */
		~GUIImage() {}

		/**
		 * \brief Method called by incoming messages.
		 * \param type Who should receive the Message.
		 * \param message Message struct.
		 */
		void update(uint16_t type, api::gui::GUIUpdateMessageStruct * message) override;

	private:
		std::vector<std::pair<std::string, std::string>> _imageSequence;
		double _fps;
		bool _looping;
		uint64_t _startTime;

		/**
		 * \brief Sets the image of the image
		 * \param ImageSet-Name
		 * \param Image-Name
		 */
		void setImage(const std::string & imageSetName, const std::string & imageName);

		void setImageSequence(const std::vector<std::pair<std::string, std::string>> & sequence, double fps, bool looping);

		void tick() override;

		/**
		 * \brief forbidden
		 */
		GUIImage(const GUIImage &) = delete;

		/**
		 * \brief forbidden
		 */
		GUIImage & operator=(const GUIImage &) = delete;
	};

} /* namespace modules */
} /* namespace i6e */

#endif /* __I6ENGINE_MODULES_GUIIMAGE_H__ */

/**
 * @}
 */
